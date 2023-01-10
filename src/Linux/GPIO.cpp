/*
 * GPIO.cpp
 *
 *  Created on: Dec 18, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef __linux__
#error This file must be compiled for LINUX target platform. Check the project.
#endif


#include <hal/GPIO.h>
#include <hal/System.h>
#include <hal/Types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <gpiod.h>


namespace dhcom
{

class GPIOImpl
{
private:
    virtual STATUS open() = 0;
    virtual STATUS close() = 0;
    virtual bool isOpen() const = 0;

    virtual STATUS setDirection(GPIO::DIRECTION dir) = 0;
    virtual STATUS set(bool value) = 0;
    virtual bool get(STATUS *status) const = 0;
    virtual int16_t pin() const = 0;

    virtual STATUS edgeDetect(GPIO::EDGE edge) = 0;
    virtual STATUS edgeWait(const struct timespec *timeout) = 0;
    virtual int fileDescriptor() const  = 0;

    friend class GPIO;
};

class GPIOfsImpl : GPIOImpl
{
private:
    GPIOfsImpl(const System &sys, GPIO::PORT pin);
    GPIOfsImpl(uint16_t pinInternal);
    ~GPIOfsImpl();

    STATUS open();
    STATUS close();
    bool isOpen() const;

    STATUS setDirection(GPIO::DIRECTION dir);
    STATUS set(bool value);
    bool get(STATUS *status) const;
    int16_t pin() const { return pin_; }

    STATUS edgeDetect(GPIO::EDGE edge);
    STATUS edgeWait(const struct timespec *timeout);
    int fileDescriptor() const { return fileDescriptor_; }

    int 	fileDescriptor_;
    int16_t 	pin_;
    STATUS 	hwStatus_;

    friend class GPIO;
};

class GPIOdImpl : GPIOImpl
{
private:
    GPIOdImpl(std::string line_name, std::string pin_name);
    ~GPIOdImpl();

    STATUS open();
    STATUS close();
    bool isOpen() const;

    STATUS setDirection(GPIO::DIRECTION dir);
    STATUS set(bool value);
    inline bool get(STATUS *status) const;
    int16_t pin() const { return -1; } // not supported for libgpiod

    STATUS edgeDetect(GPIO::EDGE edge);
    STATUS edgeWait(const struct timespec *timeout);
    int fileDescriptor() const;

    std::string 	line_name_;
    std::string 	name_;  // for name of consumer
    struct gpiod_chip 	*chip_;
    struct gpiod_line 	*line_;
    STATUS 		hwStatus_;

    friend class GPIO;
};
}


using namespace dhcom;


// GPIO::


GPIO::GPIO(const System &sys, GPIO::PORT pin)
    : impl_(NULL)
{
    System::HARDWARE hw = sys.getHardware();

    if (hw == System::HARDWARE_DHCOM_AM33 || hw == System::HARDWARE_DHCOM_AM35 ||
        hw == System::HARDWARE_DHCOM_IMX6 || hw == System::HARDWARE_DHCOM_IMX6_REV200|| hw == System::HARDWARE_DHCOM_IMX25)
    {
        impl_ = new GPIOfsImpl(sys, pin);
    }
    else
    {   // assume all the "new" DHCOMs have labeled gpios
        const char *letter[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I",
                                 "J", "K", "L", "M", "N", "O", "P", "Q", "R",
                                 "S", "T", "U", "V", "W", "X", "Y", "Z", };

        std::string gpio_name = "DHCOM-";

        impl_ = new GPIOdImpl(gpio_name.append(std::string(letter[pin])), "dhcom-hal");
    }
}


GPIO::GPIO(uint16_t pinInternal)
    : impl_(new GPIOfsImpl(pinInternal))
{
}


GPIO::~GPIO()
{
    delete impl_;
}


STATUS GPIO::open()
{
    return impl_->open();
}


STATUS GPIO::close()
{
    return impl_->close();
}


bool GPIO::isOpen() const
{
    return impl_->isOpen();
}


STATUS GPIO::setDirection(DIRECTION dir)
{
    return impl_->setDirection(dir);
}


STATUS GPIO::set(bool value)
{
    return impl_->set(value);
}


bool GPIO::get(STATUS *status) const
{
    return impl_->get(status);
}


int16_t GPIO::pin() const
{
    return impl_->pin();
}


STATUS GPIO::edgeDetect(GPIO::EDGE edge)
{
    return impl_->edgeDetect(edge);
}

STATUS GPIO::edgeWait(const struct timespec *timeout)
{
    return impl_->edgeWait(timeout);
}

int GPIO::fileDescriptor() const
{
    return impl_->fileDescriptor();
}


// GPIOfsImpl::


GPIOfsImpl::GPIOfsImpl(const System &sys, GPIO::PORT port)
    : fileDescriptor_(-1)
    , pin_(-1)
    , hwStatus_(STATUS_SUCCESS)
{
    pin_ = sys.getGPIOPortPin(port, &hwStatus_);
}


GPIOfsImpl::GPIOfsImpl(uint16_t pinInternal)
    : fileDescriptor_(-1)
    , pin_(pinInternal)
    , hwStatus_(STATUS_SUCCESS)
{
}


GPIOfsImpl::~GPIOfsImpl()
{
    close();
}


STATUS GPIOfsImpl::open()
{
    if(hwStatus_)
        return hwStatus_;

    if(isOpen())
        return STATUS_DEVICE_ALREADY_OPEN;

    // exporting
    {
        int file = ::open("/sys/class/gpio/export", O_WRONLY);
        if (file < 0)
            return STATUS_DEVICE_OPEN_FAILED;

        char strGPIOnr[4];
        snprintf(strGPIOnr, 4, "%d", pin_);

        ::write(file, strGPIOnr, 4);
        ::close(file);
    }

    // setting active_low to 0
    {
        char inv[64];
        snprintf(inv, sizeof(inv), "/sys/class/gpio/gpio%d/active_low", pin_);
        int file = ::open(inv, O_RDWR);
        if (file > 0)
        {
            ::write(file, "0", 1);
            ::close(file);
        }
    }

    // open the value file
    {
        char name[64];
        snprintf(name, sizeof(name), "/sys/class/gpio/gpio%d/value", pin_);
        fileDescriptor_ = ::open(name, O_RDWR);
        if (fileDescriptor_ < 0)
            return STATUS_DEVICE_OPEN_FAILED;
    }
    return STATUS_SUCCESS;
}


STATUS GPIOfsImpl::close()
{
    if(isOpen())
    {
        ::close(fileDescriptor_);
        fileDescriptor_ = -1;

        int file = ::open("/sys/class/gpio/unexport", O_WRONLY);
        if (file < 0)
            return STATUS_DEVICE_CLOSE_FAILED;

        char strGPIOnr[4];
        snprintf(strGPIOnr, 4, "%d", pin_);

        write(file, strGPIOnr, 4);
        ::close(file);
    }
    return STATUS_SUCCESS;
}


bool GPIOfsImpl::isOpen() const
{
    return fileDescriptor_ > 0;
}


STATUS GPIOfsImpl::setDirection(GPIO::DIRECTION dir)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    char strGPIOdirectionFile[34];
    snprintf(strGPIOdirectionFile, sizeof(strGPIOdirectionFile), "/sys/class/gpio/gpio%d/direction", pin_);

    /* open direction-file */
    int file = ::open(strGPIOdirectionFile, O_RDWR);
    if (file < 0)
        return STATUS_DEVICE_CONFIG_FAILED;

    const char *directionName;
    int directionBytes;
    if (dir == GPIO::DIRECTION_INPUT)
    {
        directionName = "in";
        directionBytes = 3;
    }
    else
    {
        directionName = "out";
        directionBytes = 4;
    }
    int tmp = write(file, directionName, directionBytes);
    ::close(file);

    /* check if the direction was written properly */
    if (tmp < directionBytes)
        return STATUS_DEVICE_CONFIG_FAILED;

    return STATUS_SUCCESS;
}


STATUS GPIOfsImpl::set(bool value)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    lseek(fileDescriptor_, 0, SEEK_SET);
    if (::write(fileDescriptor_, value ? "1" : "0", 2) != 2)
        return STATUS_DEVICE_WRITE_FAILED;

    return STATUS_SUCCESS;
}


bool GPIOfsImpl::get(STATUS *status) const
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    lseek(fileDescriptor_, 0, SEEK_SET);

    /* read value */
    char strValue[2];
    if (::read(fileDescriptor_, strValue, 2) < 0)
    {
        if(status) *status = STATUS_DEVICE_READ_FAILED;
        return false;
    }

    switch(strValue[0])
    {
    case '0':
        if(status) *status = STATUS_SUCCESS;
        return false;
    case '1':
        if(status) *status = STATUS_SUCCESS;
        return true;
    default:
        if(status) *status = STATUS_DEVICE_READ_FAILED;
        return false;
    }
}


STATUS GPIOfsImpl::edgeDetect(GPIO::EDGE edge)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    int file = ::open("/sys/class/gpio/edge", O_WRONLY);
    if (file < 0)
        return STATUS_DEVICE_CONFIG_FAILED;

    const char *data;
    int length;
    switch(edge)
    {
    default:
    case GPIO::EDGE_NONE:
        data = "none";
        length = 4;
        break;
    case GPIO::EDGE_RISING:
        data = "rising";
        length = 6;
        break;
    case GPIO::EDGE_FALLING:
        data = "falling";
        length = 7;
        break;
    case GPIO::EDGE_BOTH:
        data = "both";
        length = 4;
        break;
    }
    int written = write(file, data, length);
    ::close(file);
    return (written == length) ? STATUS_SUCCESS : STATUS_DEVICE_CONFIG_FAILED;
}

STATUS GPIOfsImpl::edgeWait(const struct timespec *timeout)
{
    // TODO
    return STATUS_SUCCESS;
}

// GPIOdImpl::

GPIOdImpl::GPIOdImpl(std::string line_name, std::string pin_name)
    : line_name_(line_name)
    , name_(pin_name)
    , chip_(NULL)
    , line_(NULL)
    , hwStatus_(STATUS_SUCCESS)
{

}


GPIOdImpl::~GPIOdImpl()
{
    close();
}


STATUS GPIOdImpl::open()
{
    int ret;

    if(hwStatus_)
        return hwStatus_;

    if(isOpen())
        return STATUS_DEVICE_ALREADY_OPEN;

    unsigned int offset;
    char gpiochip[32];

    // find gpiochip and line offset
    ret = gpiod_ctxless_find_line(line_name_.c_str(), gpiochip, sizeof(gpiochip), &offset);
    if (ret < 0) // Error
        return STATUS_DEVICE_OPEN_FAILED;
    else if (ret == 0) // gpio not found
        return STATUS_GPIO_FIND_FAILED;

    chip_ = gpiod_chip_open_by_name(gpiochip);
    if (!chip_)
        return STATUS_DEVICE_OPEN_FAILED;

    line_ = gpiod_chip_get_line(chip_, offset);
    if (!line_) {
        gpiod_chip_close(chip_);
        return STATUS_DEVICE_OPEN_FAILED;
    }

    if (gpiod_line_request_input(line_, name_.c_str()))
        return STATUS_DEVICE_CONFIG_FAILED;

    return STATUS_SUCCESS;
}


STATUS GPIOdImpl::close()
{
    if(isOpen())
    {
        if (gpiod_line_is_requested(line_))
            gpiod_line_release(line_);

        gpiod_chip_close(chip_);
        chip_ = NULL;
    }

    return STATUS_SUCCESS;
}


bool GPIOdImpl::isOpen() const
{
    return chip_ != NULL;
}


STATUS GPIOdImpl::setDirection(GPIO::DIRECTION dir)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    if (gpiod_line_is_requested(line_))
    {
        int direction = gpiod_line_direction(line_);
        if ((GPIOD_LINE_DIRECTION_INPUT == direction && dir == GPIO::DIRECTION_INPUT) ||
            (GPIOD_LINE_DIRECTION_OUTPUT == direction && dir == GPIO::DIRECTION_OUTPUT))
        {
            // pin is already configured
            return STATUS_SUCCESS;
        }
        else
        {
            gpiod_line_release(line_);
        }
    }

    if (dir == GPIO::DIRECTION_INPUT)
    {
        // input
        if (gpiod_line_request_input(line_, name_.c_str()))
            return STATUS_DEVICE_CONFIG_FAILED;
    }
    else
    {
        // output
        if (gpiod_line_request_output(line_, name_.c_str(), 0)) // default state 0
            return STATUS_DEVICE_CONFIG_FAILED;
    }

    return STATUS_SUCCESS;
}


STATUS GPIOdImpl::set(bool value)
{
    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    if (gpiod_line_set_value(line_, int(value)))
        return STATUS_DEVICE_WRITE_FAILED;

    return STATUS_SUCCESS;
}


bool GPIOdImpl::get(STATUS *status) const
{
    int ret;

    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    ret = gpiod_line_get_value(line_);
    if (ret < 0)
    {
        if (status) *status = STATUS_DEVICE_READ_FAILED;
        return false;
    }

    switch(ret)
    {
    case 0:
        if (status) *status = STATUS_SUCCESS;
        return false;
    case 1:
        if (status) *status = STATUS_SUCCESS;
        return true;
    default:
        if (status) *status = STATUS_DEVICE_READ_FAILED;
        return false;
    }
}


STATUS GPIOdImpl::edgeDetect(GPIO::EDGE edge)
{
    int ret;

    if(!isOpen())
        return STATUS_DEVICE_NOT_OPEN;

    switch(edge)
    {
    default:
    case GPIO::EDGE_NONE:
        // nothing to do
        break;
    case GPIO::EDGE_RISING:
        ret = gpiod_line_request_rising_edge_events(line_, name_.c_str());
        break;
    case GPIO::EDGE_FALLING:
        ret = gpiod_line_request_falling_edge_events(line_, name_.c_str());
        break;
    case GPIO::EDGE_BOTH:
        ret = gpiod_line_request_both_edges_events(line_, name_.c_str());
        break;
    }
    if (ret)
        return STATUS_DEVICE_CONFIG_FAILED;

    return STATUS_SUCCESS;
}

STATUS GPIOdImpl::edgeWait(const struct timespec *timeout)
{
    int ret = gpiod_line_event_wait(line_, timeout);
    if (ret < 0)
        return STATUS_DEVICE_READ_FAILED;
    else if (ret == 0)
        return STATUS_GPIO_EVENT_WAIT_TIMEOUT;

    return STATUS_SUCCESS;
}

int GPIOdImpl::fileDescriptor() const
{
    return gpiod_line_event_get_fd(line_);
}


