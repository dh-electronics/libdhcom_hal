// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2013 DH electronics GmbH
/*
 *  Created on: Jan 14, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#include <hal/System.h>
#include <stddef.h>


namespace dhcom
{


struct HardwareProps
{
	const System::HARDWARE	hardware_;
	const uint8_t 			numberOfGpioPins_;
	const uint8_t 	* const GPIOpins_;
	const uint8_t			numberOfUarts_;
	const char * const * const uartNames_;
} * System::hwProps_ = NULL;


#include "HardwareProps_PC.hpp"

System::HARDWARE System::detectSoC()
{
	return HARDWARE_PC;
}


System::System()
{
}


System::System(System::HARDWARE hardware)
{
	setHardware(hardware);
}


STATUS System::setHardware(System::HARDWARE hardware)
{
	if(hwProps_ != NULL)
		return STATUS_HARDWARE_ALREADY_DEFINED;

	if(hardware == HARDWARE_PC)
		hwProps_ = PC_Props;

	return hwProps_? STATUS_SUCCESS : STATUS_HARDWARE_UNDEFINED;
}


System::HARDWARE System::getHardware() const
{
	if(!hwProps_)
		return HARDWARE_INVALID;

	return hwProps_->hardware_;
}


uint16_t System::getUartsNumber(STATUS *status) const
{
	if(!hwProps_)
	{
		if(status) *status = STATUS_HARDWARE_UNDEFINED;
		return 0;
	}

	if(status) *status = STATUS_SUCCESS;
	return hwProps_->numberOfUarts_;
}


const char *System::getUartDeviceName(UART::DEVICE device, STATUS *status) const
{
	if(!hwProps_)
	{
		if(status) *status = STATUS_HARDWARE_UNDEFINED;
		return NULL;
	}

	if(device >= hwProps_->numberOfUarts_)
	{
		if(status) *status = STATUS_DEVICE_DOESNT_EXIST;
		return NULL;
	}

	if(status) *status = STATUS_SUCCESS;
	return hwProps_->uartNames_[device];
}


uint16_t 	System::getGPIOPortsNumber(STATUS *status) const
{
	if(!hwProps_)
	{
		if(status) *status = STATUS_HARDWARE_UNDEFINED;
		return 0;
	}

	if(status) *status = STATUS_SUCCESS;
	return hwProps_->numberOfGpioPins_;
}


uint8_t System::getGPIOPortPin(GPIO::PORT port, STATUS *status) const
{
	if(!hwProps_)
	{
		if(status) *status = STATUS_HARDWARE_UNDEFINED;
		return 0;
	}

	if(port >= hwProps_->numberOfGpioPins_)
	{
		if(status) *status = STATUS_DEVICE_DOESNT_EXIST;
		return 0;
	}

	if(status) *status = STATUS_SUCCESS;
	return hwProps_->GPIOpins_[port];
}


const char * System::getI2CDeviceName(I2CBus::BUS /* bus */) const
{
    return NULL;
}


}

