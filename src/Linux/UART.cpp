/*
 * UART.cpp
 *
 *  Created on: Dec 18, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef __linux__
#error This file must be compiled for LINUX target platform. Check the project.
#endif


#include <hal/UART.h>
#include <hal/System.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>


namespace dhcom
{


class UARTImpl
{
private:
    UARTImpl(const System &hw, UART::DEVICE device);
    UARTImpl(const char *deviceName);
    ~UARTImpl();

    void uartDeviceName(char *deviceName, quint32 size) const;

    inline STATUS setCommParams(UART::BAUDRATE baudRate, UART::PARITY parity, UART::STOPBITS stopBits, UART::FLOWCONTROL flowControl);

    inline STATUS 		open();
    inline STATUS 		close();
    inline bool			isOpen() const;
    inline quint32 write(const uint8_t *buffer, quint32 size, STATUS *status);
    inline quint32 read(uint8_t *buffer, quint32 size, STATUS *status);

    inline STATUS setPin(UART::OPIN pin, bool value);
    inline bool getPin(UART::IPIN pin, STATUS *status) const;

    const char *deviceName_;
    int 		deviceHandle_;
    STATUS		hwStatus_;

    friend class UART;
};


static int pinToBit(UART::IPIN pin)
{
    switch(pin)
    {
    case UART::IPIN_CD:
        return TIOCM_CD;
    case UART::IPIN_CTS:
        return TIOCM_CTS;
    case UART::IPIN_DSR:
        return TIOCM_DSR;
    case UART::IPIN_RI:
        return TIOCM_RI;
    }
    return -1;
}


static int pinToBit(UART::OPIN pin)
{
    switch(pin)
    {
    case UART::OPIN_RTS:
        return TIOCM_RTS;
    case UART::OPIN_DTR:
        return TIOCM_DTR;
    }
    return -1;
}


// UART::


UART::UART(const System &sys, UART::DEVICE device)
    : impl_(new dhcom::UARTImpl(sys, device))
{
}


UART::UART(const char *deviceName)
    : impl_(new dhcom::UARTImpl(deviceName))
{
}


UART::~UART()
{
    delete impl_;
}


STATUS UART::setCommParams(UART::BAUDRATE baudRate, UART::PARITY parity, UART::STOPBITS stopBits, UART::FLOWCONTROL flowControl)
{
    return impl_->setCommParams(baudRate, parity, stopBits, flowControl);
}


STATUS UART::open()
{
    return impl_->open();
}


STATUS UART::close()
{
    return impl_->close();
}


bool UART::isOpen() const
{
    return impl_->isOpen();
}


quint32 UART::write(const uint8_t *buffer, quint32 size, STATUS *status)
{
    return impl_->write(buffer, size, status);
}


quint32 UART::read(uint8_t *buffer, quint32 size, STATUS *status)
{
    return impl_->read(buffer, size, status);
}


STATUS UART::setPin(UART::OPIN pin, bool value)
{
    return impl_->setPin(pin, value);
}


bool UART::getPin(UART::IPIN pin, STATUS *status) const
{
    return impl_->getPin(pin, status);
}


// UARTImpl::


UARTImpl::UARTImpl(const System &sys, UART::DEVICE device)
    : deviceHandle_(-1)
{
    deviceName_ = sys.getUartDeviceName(device, &hwStatus_);
}


UARTImpl::UARTImpl(const char *deviceName)
    : deviceName_(deviceName)
    , deviceHandle_(-1)
    , hwStatus_(STATUS_SUCCESS)
{
}


UARTImpl::~UARTImpl()
{
    close();
}


STATUS UARTImpl::setCommParams(UART::BAUDRATE baudRate, UART::PARITY parity, UART::STOPBITS stopBits, UART::FLOWCONTROL flowControl)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    struct termios params;
    if (ioctl(deviceHandle_, TCGETS, &params) == -1)
        return STATUS_DEVICE_CONFIG_FAILED;

    // resetting unnecessary flags
    params.c_cflag &= ~(CSIZE | PARODD | CSTOPB | CRTSCTS | PARENB | CBAUD);
    params.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON | IXOFF | IXANY);
    params.c_oflag &= ~(OPOST);
    params.c_lflag &= ~(ECHO | ECHOE | ICANON | ISIG);
    params.c_cc[VMIN]  = params.c_cc[VTIME] = 0;

    // setting necessary flags
    params.c_cflag |= (CLOCAL | CREAD | CS8);	// 8 bit data size

    speed_t portBaudRate;
    switch(baudRate)
    {
    case UART::BAUDRATE_110:
        portBaudRate = B110;
        break;
    case UART::BAUDRATE_300:
        portBaudRate = B300;
        break;
    case UART::BAUDRATE_600:
        portBaudRate = B600;
        break;
    case UART::BAUDRATE_1200:
        portBaudRate = B1200;
        break;
    case UART::BAUDRATE_2400:
        portBaudRate = B2400;
        break;
    case UART::BAUDRATE_4800:
        portBaudRate = B4800;
        break;
    case UART::BAUDRATE_9600:
        portBaudRate = B9600;
        break;
    case UART::BAUDRATE_19200:
        portBaudRate = B19200;
        break;
    case UART::BAUDRATE_38400:
        portBaudRate = B38400;
        break;
    case UART::BAUDRATE_57600:
        portBaudRate = B57600;
        break;
    default:
    case UART::BAUDRATE_115200:
        portBaudRate = B115200;
        break;
    }
    cfsetispeed(&params, portBaudRate);
    cfsetospeed(&params, portBaudRate);

    // parity
    switch(parity)
    {
    case UART::PARITY_NONE:
        params.c_iflag |= IGNPAR; 			// ignore the parity errors
        break;
    case UART::PARITY_ODD:
        params.c_iflag |= INPCK;			// enable parity check, will replace parity errors with zero symbols
        params.c_cflag |= (PARENB | PARODD);// enable parity bit generation
        break;
    case UART::PARITY_EVEN:
        params.c_iflag |= INPCK;			// enable parity check, will replace parity errors with zero symbols
        params.c_cflag |= PARENB;			// enable parity bit generation
        break;
    }

    // stop bits 1 or 2
    switch(stopBits)
    {
    case UART::STOPBITS_1:
        break;
    case UART::STOPBITS_2:
        params.c_cflag |= CSTOPB;
        break;
    }

    // flow control
    switch(flowControl)
    {
    case UART::FLOW_NONE:
        break;
    case UART::FLOW_RTS_CTS:
        params.c_cflag |= CRTSCTS;
        break;
    case UART::FLOW_XON_XOFF:
        params.c_iflag |= (IXON | IXOFF | IXANY);
        break;
    }

    if(0 > ioctl(deviceHandle_, TCSETS, &params))
        return STATUS_DEVICE_CONFIG_FAILED;

    return STATUS_SUCCESS;
}


STATUS UARTImpl::open()
{
    if(hwStatus_)
        return hwStatus_;

    if(isOpen())
        return STATUS_DEVICE_ALREADY_OPEN;

    deviceHandle_ = ::open(deviceName_, O_RDWR | O_NOCTTY | O_NONBLOCK );
    if(deviceHandle_ <= 0)
        return STATUS_DEVICE_OPEN_FAILED;

    return STATUS_SUCCESS;
}


STATUS UARTImpl::close()
{
    if(isOpen())
    {
        ::close(deviceHandle_);
        deviceHandle_ = -1;
    }

    return STATUS_SUCCESS;
}


bool UARTImpl::isOpen() const
{
    return deviceHandle_ > 0;
}


quint32 UARTImpl::write(const uint8_t *buffer, quint32 size, STATUS *status)
{
    if(!isOpen())
    {
        if(status)
            *status = STATUS_DEVICE_NOT_OPEN;
        return -1;
    }

    ssize_t result = ::write(deviceHandle_, buffer, size);
    if(status)
        *status = (result >= 0) ? STATUS_SUCCESS : STATUS_DEVICE_WRITE_FAILED;
    return result;
}


quint32 UARTImpl::read(uint8_t *buffer, quint32 size, STATUS *status)
{
    if(!isOpen())
    {
        if(status)
            *status = STATUS_DEVICE_NOT_OPEN;
        return -1;
    }

    ssize_t result = ::read(deviceHandle_, buffer, size);
    if(status)
        *status = (result >= 0) ? STATUS_SUCCESS : STATUS_DEVICE_READ_FAILED;
    return result;
}


STATUS UARTImpl::setPin(UART::OPIN pin, bool value)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    int bit = pinToBit(pin);
    int status;
    if(0 > ioctl(deviceHandle_, TIOCMGET, &status))
        return STATUS_DEVICE_WRITE_FAILED;

    if(value)
        status |= bit;
    else
        status &= ~bit;

    if(0 > ioctl(deviceHandle_, TIOCMSET, &status))
        return STATUS_DEVICE_WRITE_FAILED;

    return STATUS_SUCCESS;
}


bool UARTImpl::getPin(UART::IPIN pin, STATUS *status) const
{
    if(!isOpen())
    {
        if(status)
            *status = STATUS_DEVICE_NOT_OPEN;
        return false;
    }

    int result;
    if(0 > ioctl(deviceHandle_, TIOCMGET, &result))
    {
        if(status)
            *status = STATUS_DEVICE_READ_FAILED;
        return false;
    }

    if(status)
        *status = STATUS_SUCCESS;
    return result & pinToBit(pin);
}


}
