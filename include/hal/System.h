/*
 * System.h
 *
 *  Created on: Jan 14, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef SYSTEM_H_
#define SYSTEM_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>
#include <hal/UART.h>
#include <hal/GPIO.h>
#include <hal/I2CBus.h>
#include <hal/SPI.h>


/// Use this definition if you want the library to support DHCOM-iMX6-rev200
//#define USE_HARDWARE_DHCOM_IMX6_REV200
/// Use one of these definitions if you want the library to support DHCOM-iMX6-rev300
//#define USE_HARDWARE_DHCOM_IMX6_REV300
//#define USE_HARDWARE_DHCOM_IMX6
/// Use this definition if you want the library to support DHCOM-iMX25
//#define USE_HARDWARE_DHCOM_IMX25
/// Use this definition if you want the library to support DHCOM-AM35xx
//#define USE_HARDWARE_DHCOM_AM35
/// Use this definition if you want the library to support x86 PC
//#define USE_HARDWARE_PC


namespace dhcom
{


struct HardwareProps;


class DHCOM_HAL System
{
public:
	// Enumeration for all possible hardware platforms
	enum HARDWARE
	{
        HARDWARE_INVALID            = 0,    ///< hardware was not set yet (used as a return value)
        HARDWARE_DHCOM_IMX25        = 1,    ///< Running on DHCOM-iMX25 module
        HARDWARE_DHCOM_AM35         = 2,	///< Running on DHCOM-AM35 module
        HARDWARE_DHCOM_IMX6_REV200  = 3,	///< Running on DHCOM-iMX6 module, revision 200
        HARDWARE_DHCOM_IMX6_REV300  = 4,	///< Running on DHCOM-iMX6 module, revision 300
        HARDWARE_DHCOM_IMX6         = HARDWARE_DHCOM_IMX6_REV300,   ///< Running on DHCOM-iMX6 module, shortcut for revision 300
        HARDWARE_DHCOM_AM33         = 5,	///< Running on DHCOM-AM33 module
        HARDWARE_PC             = 100   ///< Running on x86_32 or x86_64 PC
	};

	System();
    explicit System(HARDWARE hardware);

	STATUS 		setHardware(HARDWARE hardware);
	HARDWARE 	getHardware() const;

	uint16_t 	getUartsNumber(STATUS *status = NULL) const;
	const char *getUartDeviceName(UART::DEVICE device, STATUS *status = NULL) const;

	uint16_t 	getGPIOPortsNumber(STATUS *status = NULL) const;
	uint8_t 	getGPIOPortPin(GPIO::PORT port, STATUS *status = NULL) const;
    const char *getI2CDeviceName(I2CBus::BUS bus) const;
    const char *getSPIDeviceName(SPI::DEVICE bus) const;

private:
    static HardwareProps *hwProps_;
};


/** \file
 *  \brief Definition of class System.
 */


/** \class System
 *  \brief Class representing particular HW/SW combination, from which the I/O implementation may depend.
 *
 *  The I/O classes which depend on the System require the System object in their constructor args.
 */


/** \fn void System::System();
 *	\brief The class which represents the target hardware.
 *
 *	This class provides some flexibility to configure in-runtime on which target hardware we run.
 *	For example, both DHCOM module types run under linux-arm, so the same library binary can work for both,
 *	and nicely abstract from the particular module type, given the System object is correctly created.
 */


/** \fn System::System();
 *	\brief Constructor of a System object, undefined hardware.
 *
 *	The hardware my be defined later by call to System::setHardware().
 */


/** \fn System::System(System::HARDWARE hardware);
 *	\brief Constructor of a System object with target hardware specification.
 *
 *	\param hardware - the assumed target hardware.
 */


/** \fn STATUS System::setHardware(System::HARDWARE hardware);
 *	\brief Sets the target hardware.
 *
 *  The hardware can only be set if it was not set before - by this call or in constructor.
 *	\param hardware - the assumed target hardware.
 *  \return - may be STATUS_SUCCESS, STATUS_HARDWARE_ALREADY_DEFINED, STATUS_HARDWARE_UNSUPPORTED (library was not compiled for this hardware)
 */


/** \fn System::HARDWARE System::getHardware();
 *	\brief Returns the target hardware.
 *
 *	\return - can be HARDWARE_INVALID (hardware not set), or other value from System::HARDWARE enumeration.
 */


/** \fn uint16_t System::getUartsNumber(STATUS *status = NULL) const;
 *  \brief Get the number of supported hardware UARTS.
 *
 *  \param status - if NULL, status is not returned, otherwise the status will be assigned to STATUS_SUCCESS or STATUS_HARDWARE_UNDEFINED
 *  \return - number of hardware UARTS, supported by the configured target. In case of error 0 will be returned.
 */


/** \fn const char *System::getUartDeviceName(UART::DEVICE device, STATUS *status = NULL) const;
 *  \brief Get the OS-dependent device name for UART.
 *
 *  \param device - which device to look for.
 *  \param status - if NULL, status is not returned, otherwise the status will be assigned to STATUS_SUCCESS, STATUS_HARDWARE_UNDEFINED or STATUS_DEVICE_DOESNT_EXIST.
 *  \return - a const string defining the device file name.
 */


/** \fn uint16_t 	System::getGPIOPortsNumber(STATUS *status = NULL) const;
 *  \brief Get the number of supported GPIO ports.
 *
 *  \param status - if NULL, status is not returned, otherwise the status will be assigned to STATUS_SUCCESS or STATUS_HARDWARE_UNDEFINED
 *  \return - number of supported GPIO ports.
 */


/** \fn uint8_t 	System::getGPIOPortPin(GPIO::PORT port, STATUS *status = NULL) const;
 *  \brief
 *
 *  \param port -  the port for which to get the pin number.
 *  \param status - if NULL, status is not returned, otherwise the status will be assigned to STATUS_SUCCESS, STATUS_HARDWARE_UNDEFINED or STATUS_DEVICE_DOESNT_EXIST.
 *  \return -  the pin number related to the port (hardware-dependent). In case of an error 0 is returned.
 */


}


#endif /* SYSTEM_H_ */
