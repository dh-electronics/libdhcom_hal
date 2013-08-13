/*
 * ADC.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef __linux__
#error This file must be compiled for LINUX target platform. Check the project.
#endif


#include <hal/ADC.h>
#include <hal/System.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include "imx_adc.h"
#include <string>


using namespace std;
namespace dhcom
{


class ADCImpl
{
public:
	ADCImpl(const System &sys, ADC::CHANNEL channel);
	~ADCImpl();

	void open();
	void close();
	bool isOpen() const { return fd_ > 0; }

	uint16_t read() const;

private:
	const char * deviceName_;
	int fd_;
	t_channel	channel_;
};


ADC::ADC(const System &sys, ADC::CHANNEL channel)
: impl_(new ADCImpl(sys, channel))
{
}


ADC::~ADC()
{
	delete impl_;
}



void ADC::open()
{
	impl_->open();
}


void ADC::close()
{
	impl_->close();
}


bool ADC::isOpen() const
{
	return impl_->isOpen();
}


uint16_t ADC::read() const
{
	return impl_->read();
}


ADCImpl::ADCImpl(const System &sys, ADC::CHANNEL channel)
: deviceName_(NULL)
, fd_(-1)
{
	// depending on the dhcom type we will use different device
	switch(sys.getHardware())
	{
		case System::HARDWARE_DHCOM_IMX25:
			deviceName_ = "/dev/imx_adc";
			break;

		case System::HARDWARE_DHCOM_AM35:
			throw string("DHCOM AM35xx currently does not support ADC");

		default:
			throw string("Hardware does not support ADC");
	}

	switch(channel)
	{
	case ADC::CHANNEL_0:
		channel_ = GER_PURPOSE_ADC0;
		break;

	case ADC::CHANNEL_1:
		channel_ = GER_PURPOSE_ADC1;
		break;

	case ADC::CHANNEL_2:
		channel_ = GER_PURPOSE_ADC2;
		break;

	default:
		throw string("ADC channel not supported");
	}
}


ADCImpl::~ADCImpl()
{
	close();
}


void ADCImpl::open()
{
	if(isOpen())
		throw string("Trying to open an already open ADC.");

	fd_ = ::open(deviceName_, O_RDONLY);
	if(fd_ < 0)
		throw string("ADC device could not be opened");

	if(0 != ioctl(fd_, IMX_ADC_INIT, NULL))
		throw string("ADC initialization failed");
}


void ADCImpl::close()
{
	if(!isOpen())
		return;

	if(0 != ioctl(fd_, IMX_ADC_DEINIT, NULL))
		throw string("ADC de-initialization failed");

	::close(fd_);
	fd_ = -1;
}


uint16_t ADCImpl::read() const
{
	if(!isOpen())
		throw string("Read on a non-open ADC.");

	// read out the value on ADC
	struct t_adc_convert_param convert_param;
	convert_param.channel = channel_;
	convert_param.result[0] = 0;

	if(0 != ioctl(fd_, IMX_ADC_CONVERT, &convert_param))
		throw string("ADC conversion failed");

	//	printf("Converted value: %hu\n", convert_param.result[0]);
	return convert_param.result[0];
}


}
