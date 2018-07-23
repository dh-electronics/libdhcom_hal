/*
 * SPI.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef __linux__
#error This file must be compiled for LINUX target platform. Check the project.
#endif


#include <hal/SPI.h>
#include <hal/System.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>


namespace dhcom
{


class SPIImpl
{
private:
    SPIImpl(SPI::DEVICE device, SPI::CHIPSELECT chipSelect);
    SPIImpl(const char *device_name);
    ~SPIImpl();

    inline STATUS open();
    inline STATUS close();
    inline bool isOpen() const;

    inline STATUS setCommParams(SPI::MODE mode, uint8_t bits, uint32_t freqHZ);
    inline int transceive(const uint8_t *outputBuffer, uint8_t *inputBuffer, uint32_t count, STATUS *status);

    const char *        deviceName_;
    int 		deviceHandle_;
    uint32_t		freqHZ_;
    uint8_t		bits_;

    friend class SPI;
};


// SPI::


SPI::SPI(SPI::DEVICE device, SPI::CHIPSELECT chipSelect)
    : impl_(new SPIImpl(device, chipSelect))
{
}


SPI::SPI(const System &, SPI::DEVICE device, SPI::CHIPSELECT chipSelect)
    : impl_(new SPIImpl(device, chipSelect))
{
}


SPI::SPI(const char *devname)
    : impl_(new SPIImpl(devname))
{
}


SPI::~SPI()
{
    delete impl_;
}


STATUS SPI::open()
{
    return impl_->open();
}


STATUS SPI::close()
{
    return impl_->close();
}


bool SPI::isOpen() const
{
    return impl_->isOpen();
}


STATUS SPI::setCommParams(MODE mode, uint8_t bits, uint32_t freqHZ)
{
    return impl_->setCommParams(mode, bits, freqHZ);
}


int SPI::transceive(const uint8_t *outputBuffer, uint8_t *inputBuffer, uint32_t count, STATUS *status)
{
    return impl_->transceive(outputBuffer, inputBuffer, count, status);
}


// SPIImpl::


SPIImpl::SPIImpl(SPI::DEVICE device, SPI::CHIPSELECT chipSelect)
    : deviceName_(NULL)
    , deviceHandle_(-1)
{
    openlog("DHCOM_HAL SPI", LOG_PID|LOG_CONS|LOG_PERROR, LOG_SYSLOG);
    if(!System().getHardware())
    {
        syslog(LOG_ERR, "%s(): getHardware() failed!\n", __FUNCTION__);
        return;
    }
    deviceName_ = System().getSPIDeviceName(device);
}


SPIImpl::SPIImpl(const char *device_name)
    : deviceName_(device_name)
    , deviceHandle_(-1)
{
    openlog("DHCOM_HAL SPI", LOG_PID|LOG_CONS|LOG_PERROR, LOG_SYSLOG);
}



SPIImpl::~SPIImpl()
{
    close();
    closelog();
}


STATUS SPIImpl::open()
{
    if(isOpen())
    {
        syslog(LOG_ERR, "%s():SPI already open!\n", __FUNCTION__);
        return STATUS_DEVICE_ALREADY_OPEN;
    }

    if(!deviceName_)
    {
        syslog(LOG_ERR, "%s(): Requested device does not exist!\n", __FUNCTION__);
        return STATUS_DEVICE_DOESNT_EXIST;
    }

    deviceHandle_ = ::open(deviceName_, O_RDWR);
    if(deviceHandle_ < 0)
    {
        syslog(LOG_ERR, "%s():Open spi %s failed: %s\n", __FUNCTION__, deviceName_, strerror(errno));
        return STATUS_DEVICE_OPEN_FAILED;
    }
    /*
    if(0 > ioctl(deviceHandle_, SPI_IOC_RD_BITS_PER_WORD, &bits_))
        return STATUS_READ_BITS_FAILED;

    if (0 > ioctl(deviceHandle_, SPI_IOC_RD_MAX_SPEED_HZ, &freqHZ_))
        return STATUS_READ_FREQ_FAILED;
*/
    return STATUS_SUCCESS;
}


STATUS SPIImpl::close()
{
    if(isOpen())
    {
        ::close(deviceHandle_);
        deviceHandle_ = -1;
    }

    return STATUS_SUCCESS;
}


bool SPIImpl::isOpen() const
{
    return deviceHandle_ > 0;
}


STATUS SPIImpl::setCommParams(SPI::MODE mode, uint8_t bits, uint32_t freqHZ)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    /* spi mode	 */
    if(0 > ioctl(deviceHandle_, SPI_IOC_WR_MODE, &mode)) {
        syslog(LOG_ERR, "%s(): Failed to setup spi mode: %s\n", __FUNCTION__, strerror(errno));
        return STATUS_DEVICE_CONFIG_FAILED;
    }

    /* bit order - always MSb first*/
    uint8_t SPILSB = 0;
    if(0 > ioctl(deviceHandle_, SPI_IOC_WR_LSB_FIRST, &SPILSB)) {
        syslog(LOG_ERR, "%s(): Failed to set MSb first: %s\n", __FUNCTION__, strerror(errno));
        return STATUS_DEVICE_CONFIG_FAILED;
    }

    /* bits per word - always 8*/
    bits_ = bits;
    if(0 > ioctl(deviceHandle_, SPI_IOC_WR_BITS_PER_WORD, &bits)) {
        syslog(LOG_ERR, "%s(): Failed to set bits per word: %s\n", __FUNCTION__, strerror(errno));
        return STATUS_DEVICE_CONFIG_FAILED;
    }

    /* max speed Hz */
    freqHZ_ = freqHZ;
    if(0 > ioctl(deviceHandle_, SPI_IOC_WR_MAX_SPEED_HZ, &freqHZ)) {
        syslog(LOG_ERR, "%s(): Failed to set max speed: %s\n", __FUNCTION__, strerror(errno));
        return STATUS_DEVICE_CONFIG_FAILED;
    }

    return STATUS_SUCCESS;
}


int SPIImpl::transceive(const uint8_t *outputBuffer, uint8_t *inputBuffer, uint32_t length, STATUS *status)
{
    if(!isOpen())
    {
        if(status) *status = STATUS_DEVICE_NOT_OPEN;
        return -1;
    }

    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)outputBuffer;
    tr.rx_buf = (unsigned long)inputBuffer;
    tr.len = length;
    tr.delay_usecs = 0;
    tr.speed_hz = freqHZ_;
    tr.bits_per_word = bits_;
    tr.cs_change = 0;

    int result = ioctl(deviceHandle_, SPI_IOC_MESSAGE(1), &tr);
    if(status)
        *status = (result < 0) ? STATUS_DEVICE_WRITE_FAILED : STATUS_SUCCESS;
    return result;
}


}
