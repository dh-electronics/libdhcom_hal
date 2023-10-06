// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2013 DH electronics GmbH
/*
 *  Created on: Jan 14, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#include <hal/System.h>
#include <stddef.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>

namespace dhcom
{


struct HardwareProps
{
	const System::HARDWARE	hardware_;
	const uint8_t 			numberOfGpioPins_;
	const uint8_t 	* const GPIOpins_;
	const uint8_t			numberOfUarts_;
	const char * const * const uartNames_;
	const uint8_t			numberOfI2C_;
	const char * const * const i2cNames_;
	const uint8_t			numberOfSPI_;
	const char * const * const spiNames_;

} * System::hwProps_ = NULL;


#include "HardwareProps_AM35.hpp"
#include "HardwareProps_AM33.hpp"
#include "HardwareProps_IMX25.hpp"
#include "HardwareProps_IMX6.hpp"
#include "HardwareProps_IMX6ULL.hpp"
#include "HardwareProps_STM32MP1.hpp"
#include "HardwareProps_PC.hpp"


/*

===== Cpuinfo for AM33 =====
processor	: 0
model name	: ARMv7 Processor rev 2 (v7l)
BogoMIPS	: 495.61
Features	: half thumb fastmult vfp edsp thumbee neon vfpv3 tls vfpd32
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x3
CPU part	: 0xc08
CPU revision	: 2

Hardware	: Generic AM33XX (Flattened Device Tree)
Revision	: 0000
Serial		: 0000000000000000

===== Cpuinfo for AM35 =====
Processor	: ARMv7 Processor rev 7 (v7l)
BogoMIPS	: 597.64
Features	: swp half thumb fastmult vfp edsp thumbee neon vfpv3 tls
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x1
CPU part	: 0xc08
CPU revision	: 7

Hardware	: DHCM3517
Revision	: 0020
Serial		: 0000000000000000

===== Cpuinfo for iMX6DL =====
processor	: 0
model name	: ARMv7 Processor rev 10 (v7l)
Features	: swp half thumb fastmult vfp edsp neon vfpv3 tls vfpd32
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x2
CPU part	: 0xc09
CPU revision	: 10

processor	: 1
model name	: ARMv7 Processor rev 10 (v7l)
Features	: swp half thumb fastmult vfp edsp neon vfpv3 tls vfpd32
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x2
CPU part	: 0xc09
CPU revision	: 10

Hardware	: Freescale i.MX6 Quad/DualLite (Device Tree)
Revision	: 0000
Serial		: 0000000000000000

===== Cpuinfo for iMX6ULL =====
processor       : 0
model name      : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 96.00
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc07
CPU revision    : 5

Hardware        : Freescale i.MX6 Ultralite (Device Tree)
Revision        : 0000
Serial          : 0000000000000000

===== Cpuinfo for STM32MP1 =====
root@dhcom:~# cat /proc/cpuinfo
processor       : 0
model name      : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 48.00
Features        : half thumb fastmult vfp edsp thumbee neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc07
CPU revision    : 5

processor       : 1
model name      : ARMv7 Processor rev 5 (v7l)
BogoMIPS        : 48.00
Features        : half thumb fastmult vfp edsp thumbee neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xc07
CPU revision    : 5

Hardware        : STM32 (Device Tree Support)
Revision        : 0000
Serial          : 001F003C3438510838333630
*/

System::HARDWARE System::detectSoC()
{
	enum CORTEX_A      { CORTEX_A7, CORTEX_A8, CORTEX_A9, CPU_INVALID };
	enum HARDWARE_INFO { HW_IMX6, HW_STM32, HW_OTHER };

	System::HARDWARE hw         = System::HARDWARE_INVALID;
	enum CORTEX_A cpu           = CPU_INVALID;
	enum HARDWARE_INFO hardware = HW_OTHER;
	int cpu_variant             = -1;
	char *line                  = NULL;
	size_t size;

	FILE *file = fopen("/proc/cpuinfo", "r");
	if(!file)
		return System::HARDWARE_INVALID;

	while(getline(&line, &size, file) != -1) // parse line by line
	{
		if(std::strstr(line, "CPU implementer") && !std::strstr(line, "0x41"))
			break;                              // Not An ARM!

		if(std::strstr(line, "CPU architecture") && !std::strstr(line, "7"))
			break;                              // Unknown ARCH

		if(std::strstr(line, "CPU variant"))
		{
			if(std::strstr(line, "0x3"))
				// could be AM33
				cpu_variant = 3;
			else if(std::strstr(line, "0x1"))
				// could be AM3505 or AM3517
				cpu_variant = 1;
			else if(std::strstr(line, "0x2"))
				// could be i.MX6qdls
				cpu_variant = 2;
			else if(std::strstr(line, "0x0"))
				// could be i.MX6ULL or STM32MP1
				cpu_variant = 0;
			else
			{
				// invalid
				cpu_variant = -1;
				break;
			}
		}

		// parse Cortex-Axx
		if(std::strstr(line, "CPU part"))
		{
			if(std::strstr(line, "0xc07"))
				cpu = CORTEX_A7;
			else if(std::strstr(line, "0xc08"))
				cpu = CORTEX_A8;
			else if(std::strstr(line, "0xc09"))
				cpu = CORTEX_A9;
			else
			{
				cpu = CPU_INVALID;
				break;
			}
		}

		// parse Hardware info
		if(std::strstr(line, "Hardware"))
		{
			if(std::strstr(line, "i.MX6"))
				hardware = HW_IMX6;
			else if(std::strstr(line, "STM32"))
				hardware = HW_STM32;
			else
				hardware = HW_OTHER;
		}
	}

	delete line;
	fclose(file);

	if(cpu == CPU_INVALID)
		return System::HARDWARE_INVALID;

	switch(hardware)
	{
	case HW_IMX6:
		if (cpu_variant == 2) // i.MX6qdls
		{
			hw = System::HARDWARE_DHCOM_IMX6_REV300;
		}
		else if(cpu_variant == 0) // i.MX6ull
		{
			hw = System::HARDWARE_DHCOM_IMX6ULL;
		}
		break;
	case HW_STM32:
		if(cpu_variant == 0) // STM32MP1
		{
			hw = System::HARDWARE_DHCOM_STM32MP1;
		}
		break;
	default: // i.MX25 is not handled (not supported)
		if (cpu_variant == 3) // AM33
		{
			hw = System::HARDWARE_DHCOM_AM33;
		}
		else if(cpu_variant == 1) // AM3505 or AM3517
		{
			hw = System::HARDWARE_DHCOM_AM35;
		}
		else if(cpu_variant == 0) // STM32MP1 (used when Hardware Info field is not valid at STM32 dt)
		{
			hw = System::HARDWARE_DHCOM_STM32MP1;
		}
	}

	// check selection for cpu part Cortex-A
	switch(hw)
	{
	case System::HARDWARE_DHCOM_AM33:
		if (cpu != CORTEX_A8)
			return System::HARDWARE_INVALID;
		break;
	case System::HARDWARE_DHCOM_AM35:
		if (cpu != CORTEX_A8)
			return System::HARDWARE_INVALID;
		break;
	case System::HARDWARE_DHCOM_IMX6_REV300:
		if (cpu != CORTEX_A9)
			return System::HARDWARE_INVALID;
		break;
	case System::HARDWARE_DHCOM_IMX6ULL:
		if (cpu != CORTEX_A7)
			return System::HARDWARE_INVALID;
		break;
	case System::HARDWARE_DHCOM_STM32MP1:
		if (cpu != CORTEX_A7)
			return System::HARDWARE_INVALID;
		break;
	default: // unknown HW / CPU match
		return System::HARDWARE_INVALID;
	}

	return hw;
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

	switch(hardware)
	{
	case HARDWARE_DHCOM_IMX25:
		hwProps_ = IMX25_Props;
		break;
	case HARDWARE_DHCOM_AM35:
		hwProps_ = AM35_Props;
		break;
	case HARDWARE_DHCOM_AM33:
		hwProps_ = AM33_Props;
		break;
	case HARDWARE_DHCOM_IMX6_REV200:
		hwProps_ = IMX6_REV200_Props;
		break;
	case HARDWARE_DHCOM_IMX6_REV300:
		hwProps_ = IMX6_REV300_Props;
		break;
	case HARDWARE_DHCOM_IMX6ULL:
		hwProps_ = IMX6ULL_Props;
		break;
	case HARDWARE_DHCOM_STM32MP1:
		hwProps_ = STM32MP1_Props;
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
    if(!hwProps_ || bus >= hwProps_->numberOfI2C_)
        return NULL;

    return hwProps_->i2cNames_[uint8_t(bus)];
}


const char * System::getSPIDeviceName(SPI::DEVICE bus) const
{
    if(!hwProps_ || bus >= hwProps_->numberOfSPI_)
        return NULL;

    return hwProps_->spiNames_[uint8_t(bus)];
}


}

