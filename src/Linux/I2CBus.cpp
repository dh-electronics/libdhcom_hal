/*
 * I2C.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef __linux__
#error This file must be compiled for LINUX target platform. Check the project.
#endif


#include <hal/I2CBus.h>
#include <hal/System.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <errno.h>
#include <string.h>

using namespace std;


namespace dhcom
{


class I2CBusImpl
{
private:
    I2CBusImpl(I2CBus::BUS bus); // implicitly uses already defined System
    ~I2CBusImpl();

    inline STATUS open();
    inline STATUS close();
    inline bool isOpen() const;

    inline STATUS selectSlave(uint16_t address);
    inline STATUS read(uint8_t *buffer, quint32 count) const;
    inline STATUS write(const uint8_t *buffer, quint32 count);

    I2CBus::BUS 	bus_;
    int 			deviceHandle_;
    int 			lastSelectedSlave_;

    friend class I2CBus;
};


// I2CBus::


I2CBus::I2CBus(const System &, BUS bus)
    : impl_(new I2CBusImpl(bus))
{
}


I2CBus::I2CBus(I2CBus::BUS bus)
    : impl_(new I2CBusImpl(bus))
{
}


I2CBus::~I2CBus()
{
    delete impl_;
}


STATUS I2CBus::open()
{
    return impl_->open();
}


STATUS I2CBus::close()
{
    return impl_->close();
}


bool I2CBus::isOpen() const
{
    return impl_->isOpen();
}


STATUS I2CBus::selectSlave(uint16_t address)
{
    return impl_->selectSlave(address);
}


STATUS I2CBus::read(uint8_t *buffer, quint32 count) const
{
    return impl_->read(buffer, count);
}


uint8_t I2CBus::read(STATUS *status) const
{
    uint8_t data;
    STATUS st = impl_->read(&data, 1);
    if(status) *status = st;
    return data;
}


STATUS I2CBus::write(const uint8_t *buffer, quint32 count)
{
    return impl_->write(buffer, count);
}


STATUS I2CBus::write(uint8_t byte)
{
    return impl_->write(&byte, 1);
}


// I2CBusImpl::


I2CBusImpl::I2CBusImpl(I2CBus::BUS bus)
    : bus_(bus)
    , deviceHandle_(-1)
    , lastSelectedSlave_(-1)
{
}


I2CBusImpl::~I2CBusImpl()
{
    close();
}


STATUS I2CBusImpl::open()
{
    if(isOpen())
        return STATUS_DEVICE_ALREADY_OPEN;

    if(!System().getHardware())
        return STATUS_HARDWARE_UNDEFINED;

    const char * deviceName = System().getI2CDeviceName(bus_);
    if(!deviceName)
        return STATUS_DEVICE_DOESNT_EXIST;

    deviceHandle_ = ::open(deviceName, O_RDWR);
    if(0 > deviceHandle_)
        return STATUS_DEVICE_OPEN_FAILED;

    return STATUS_SUCCESS;
}


STATUS I2CBusImpl::close()
{
    if(isOpen())
    {
        ::close(deviceHandle_);
        deviceHandle_ = -1;
    }

    return STATUS_SUCCESS;
}


bool I2CBusImpl::isOpen() const
{
    return deviceHandle_ > 0;
}


STATUS I2CBusImpl::selectSlave(uint16_t address)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    if(lastSelectedSlave_ == int(address))
        return STATUS_SUCCESS;

    if(0 > ioctl(deviceHandle_, I2C_SLAVE, int(address)))
        return STATUS_I2C_SLAVE_SELECT_FAILED;

    lastSelectedSlave_ = address;
    return STATUS_SUCCESS;
}


STATUS I2CBusImpl::read(uint8_t *buffer, quint32 count) const
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    if(0 > ::read(deviceHandle_, buffer, count))
        return STATUS_DEVICE_READ_FAILED;

    return STATUS_SUCCESS;
}


STATUS I2CBusImpl::write(const uint8_t *buffer, quint32 count)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    if(0 > ::write(deviceHandle_, buffer, count))
        return STATUS_DEVICE_WRITE_FAILED;

    return STATUS_SUCCESS;
}


}
