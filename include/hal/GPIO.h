/*
 * GPIO.h
 *
 *  Created on: Dec 17, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef GPIO_H_
#define GPIO_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>


namespace dhcom
{


class System;
class GPIOImpl;


class DHCOM_HAL GPIO
{
public:
	/// DHCOM module GPIO ports enumeration
	enum PORT
	{
		PORT_GPIO_A = 0,
		PORT_GPIO_B, PORT_GPIO_C, PORT_GPIO_D,
		PORT_GPIO_E, PORT_GPIO_F, PORT_GPIO_G, PORT_GPIO_H,
		PORT_GPIO_I, PORT_GPIO_J, PORT_GPIO_K, PORT_GPIO_L,
		PORT_GPIO_M, PORT_GPIO_N, PORT_GPIO_O, PORT_GPIO_P,
		PORT_GPIO_Q, PORT_GPIO_R, PORT_GPIO_S, PORT_GPIO_T,
		PORT_GPIO_U, PORT_GPIO_V, PORT_GPIO_W,
	};

	/// Direction of a GPIO port (input or output)
	enum DIRECTION
	{
		DIRECTION_INPUT,	///< the pin used as input
		DIRECTION_OUTPUT,	///< the pin used as output
	};

	enum EDGE
	{
		EDGE_NONE		= 0,	///< the pin used as input
		EDGE_RISING 	= 1,	///< the pin used as output
		EDGE_FALLING 	= 2,	///< the pin used as output
		EDGE_BOTH 		= 3,	///< the pin used as output
	};

	GPIO(const System &sys, PORT port);
    explicit GPIO(uint16_t pinInternal);
	~GPIO();

	STATUS open();
	STATUS close();
	bool isOpen() const;

	STATUS setDirection(DIRECTION dir);
	STATUS set(bool value);
	bool get(STATUS *status = NULL) const;
	int16_t pin() const;

	STATUS edgeDetect(EDGE edge);
	int fileDescriptor() const;

private:
    GPIO(const GPIO &) : impl_(NULL) {}

	GPIOImpl * const impl_;
};


/** \file
 *  \brief Definitions of the GPIO class.
 *
 *  \class GPIO
 *  \brief Class provides initialization and R/W access to the discrete I/O pins of a DHCOM module.
 *
 *  For different module types different number of pins is accessible.
 *  See the online wiki at http://xlon.de/wiki/index.php?title=DHCOM.
 */


/** \fn GPIO::GPIO(const System &sys, GPIO::PIN port)
 *  \brief Constructs the GPIO object and defines which port it will represent.
 *
 *  \param sys - an initialized System object, which defines the target hardware.
 *  \param port - the port.
 */


/** \fn GPIO::GPIO(uint16_t pinInternal)
 *  \brief Constructs the GPIO object and defines which pin (internal number) it will represent.
 *
 *  \param pinInternal - the pin number, used by operating system driver.
 */


/** \fn GPIO::~GPIO()
 *  \brief Destructs the GPIO object, closing it if necessary.
 */


/** \fn STATUS GPIO::open();
 *  \brief Opens the GPIO pin for I/O operations.
 *
 *  Before any set/get operations or setting the direction the port must be open.
 *  \return - can be STATUS_SUCCESS, STATUS_DEVICE_OPEN_FAILED, STATUS_DEVICE_DOESNT_EXIST or STATUS_HARDWARE_UNDEFINED.
 */


/** \fn STATUS GPIO::close();
 *  \brief Closes the GPIO port.
 *
 *  \return - can be STATUS_SUCCESS or STATUS_DEVICE_CLOSE_FAILED.
 */


/** \fn bool GPIO::isOpen() const;
 *  \brief Checks if the GPIO port is open.
 *  \return true is the port is open.
 */


/** \fn STATUS GPIO::setDirection(GPIO::DIRECTION dir);
 *  \brief Sets the direction of the I/O port.
 *  \param dir - the direction to set.
 */


/** \fn STATUS GPIO::set(bool value)
 *  \brief Sets the output pin state.
 *  \param value - the value to set, true means high level, and false - low level.
 */


/** \fn bool GPIO::get(GPIO::STATUS *status)
 *  \brief Reads the pin state.
 *  \return - true if the pin was in high state, false otherwise
 */


/** \fn int16_t GPIO::pin() const;
 *  \brief Gets the internal pin number, represented by the port.
 *  \return - the pin number, used by the OS driver.
 *
 *  Warning: The internally used pin numbers depend on the target hardware.
 */


}


#endif /* GPIO_H_ */
