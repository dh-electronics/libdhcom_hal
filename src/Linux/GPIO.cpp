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


namespace dhcom
{
class GPIOImpl
{
private:
	GPIOImpl(const System &sys, GPIO::PORT pin);
	GPIOImpl(uint16_t pinInternal);
	~GPIOImpl();

	inline STATUS open();
	inline STATUS close();
	inline bool isOpen() const;

	inline STATUS setDirection(GPIO::DIRECTION dir);
	inline STATUS set(bool value);
	inline bool get(STATUS *status) const;
	int16_t pin() const { return pin_; }

	int 	fileDescriptor_;
	int16_t pin_;
	STATUS 	hwStatus_;

	friend class GPIO;
};
}


using namespace dhcom;


// GPIO::


GPIO::GPIO(const System &sys, GPIO::PORT pin)
: impl_(new GPIOImpl(sys, pin))
{
}


GPIO::GPIO(uint16_t pinInternal)
: impl_(new GPIOImpl(pinInternal))
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
	impl_->set(value);
}


bool GPIO::get(STATUS *status) const
{
	return impl_->get(status);
}


int16_t GPIO::pin() const
{
	return impl_->pin();
}



// GPIOImpl::


GPIOImpl::GPIOImpl(const System &sys, GPIO::PORT port)
: fileDescriptor_(-1)
, pin_(-1)
, hwStatus_(STATUS_SUCCESS)
{
	pin_ = sys.getGPIOPortPin(port, &hwStatus_);
}


GPIOImpl::GPIOImpl(uint16_t pinInternal)
: fileDescriptor_(-1)
, pin_(pinInternal)
, hwStatus_(STATUS_SUCCESS)
{
}


GPIOImpl::~GPIOImpl()
{
	close();
}


STATUS GPIOImpl::open()
{
	if(hwStatus_)
		return hwStatus_;

	if(isOpen())
		return STATUS_DEVICE_ALREADY_OPEN;

	int file = ::open("/sys/class/gpio/export", O_WRONLY);
	if (file < 0)
		return STATUS_DEVICE_OPEN_FAILED;

	char strGPIOnr[4];
	snprintf(strGPIOnr, 4, "%d", pin_);

	::write(file, strGPIOnr, 4);
	::close(file);

	// open the value file
	char name[36];
	snprintf(name, sizeof(name), "/sys/class/gpio/gpio%d/value", pin_);
	fileDescriptor_ = ::open(name, O_RDWR);
	if (fileDescriptor_ < 0)
		return STATUS_DEVICE_OPEN_FAILED;

	return STATUS_SUCCESS;
}


STATUS GPIOImpl::close()
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


bool GPIOImpl::isOpen() const
{
	return fileDescriptor_ > 0;
}


STATUS GPIOImpl::setDirection(GPIO::DIRECTION dir)
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


STATUS GPIOImpl::set(bool value)
{
	if(!isOpen())
		return STATUS_DEVICE_NOT_OPEN;

	lseek(fileDescriptor_, 0, SEEK_SET);
	if (::write(fileDescriptor_, value ? "1" : "0", 2) != 2)
		return STATUS_DEVICE_WRITE_FAILED;

	return STATUS_SUCCESS;
}


bool GPIOImpl::get(STATUS *status) const
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
