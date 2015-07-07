/*
 * DHCOM.h
 *
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
    const uint8_t   * const i2cDeviceNameIdx_;
} * System::hwProps_ = NULL;


static const char *i2cDeviceNames[] = {"/dev/i2c-1", "/dev/i2c-2", "/dev/i2c-3"};


#include "HardwareProps_AM35.hpp"
#include "HardwareProps_IMX25.hpp"
#include "HardwareProps_IMX6.hpp"
#include "HardwareProps_PC.hpp"


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

	switch(hardware)
	{
	case HARDWARE_DHCOM_IMX25:
		hwProps_ = IMX25_Props;
		break;
	case HARDWARE_DHCOM_AM35:
		hwProps_ = AM35_Props;
		break;
    case HARDWARE_DHCOM_IMX6_REV200:
        hwProps_ = IMX6_REV200_Props;
        break;
    case HARDWARE_DHCOM_IMX6_REV300:
        hwProps_ = IMX6_REV300_Props;
        break;
	case HARDWARE_PC:
		hwProps_ = PC_Props;
		break;
	default:
		break;
	}
	return hwProps_? STATUS_SUCCESS : STATUS_HARDWARE_UNSUPPORTED;
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


const char * System::getI2CDeviceName(I2CBus::BUS bus) const
{
    return i2cDeviceNames[uint8_t(bus)];
}


}

