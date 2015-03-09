/*
 * Uart.h
 *
 *  Created on: Nov 27, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef UART_H_
#define UART_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>


namespace dhcom
{


class System;
class UARTImpl;


class DHCOM_HAL UART
{
public:
	/// Enumeration of the UART devices
	enum DEVICE
	{
		DEVICE_UART0 = 0,
		DEVICE_UART1, DEVICE_UART2,	DEVICE_UART3,
		DEVICE_UART4, DEVICE_UART5,	DEVICE_UART6,
		DEVICE_UART7, DEVICE_UART8,	DEVICE_UART9,
		DEVICE_UART10, DEVICE_UART11, DEVICE_UART12,
		DEVICE_UART13, DEVICE_UART14, DEVICE_UART15,
		DEVICE_UART16, DEVICE_UART17, DEVICE_UART18,
		DEVICE_UART19,
	};

	/// Enumeration of supported baud rates
	enum BAUDRATE
	{
		BAUDRATE_110,	BAUDRATE_300,
		BAUDRATE_600, 	BAUDRATE_1200, 	BAUDRATE_2400,
		BAUDRATE_4800,	BAUDRATE_9600, 	BAUDRATE_19200, BAUDRATE_38400,
		BAUDRATE_57600,	BAUDRATE_115200,
	};

	/// Enumeration of supported parity bit configurations
	enum PARITY
	{
#undef PARITY_NONE
#undef PARITY_ODD
#undef PARITY_EVEN
		PARITY_NONE, PARITY_ODD, PARITY_EVEN,
	};

	/// Enumeration of supported stop bit configurations
	enum STOPBITS
	{
		STOPBITS_1,		///< one stop bit
		STOPBITS_2,     ///< two stop bits
	};

	/// Enumeration of supported flow control methods
 	enum FLOWCONTROL
	{
		FLOW_NONE,		///< No flow control
		FLOW_RTS_CTS,	///< Hardware flow control
		FLOW_XON_XOFF,	///< Software flow control
	};

 	/// Enumeration of UART pins, which can be used as discrete inputs
	enum IPIN
	{
		IPIN_CD, 	///< the carrier detect pin
		IPIN_CTS,	///< the Clear To Send (CTS) pin
		IPIN_DSR,	///< the DSR pin
		IPIN_RI,	///< the Ring Incoming pin
	};

 	/// Enumeration of UART pins, which can be used as discrete outputs
	enum OPIN
	{
		OPIN_RTS,	///< the Ready To Send (RTS) pin
		OPIN_DTR,	///< the DTR pin
	};

	UART(const System &sys, DEVICE device);
    explicit UART(const char *deviceName);
	~UART();

	STATUS setCommParams(BAUDRATE baudRate, PARITY parity, STOPBITS stopBits, FLOWCONTROL flowControl);

	STATUS 		open();
	STATUS 		close();
	bool		isOpen() const;
	uint32_t write(const uint8_t *buffer, uint32_t size, STATUS *status = NULL);
	uint32_t read(uint8_t *buffer, uint32_t size, STATUS *status = NULL);

	STATUS setPin(OPIN pin, bool value);
	bool getPin(IPIN pin, STATUS *status = NULL) const;

private:
    UART(const UART &) : impl_(NULL) {}

	UARTImpl * const impl_;
};



/** \file
 *  \brief Definitions of the UART class.
 *
 *  \class UART
 *  \brief Provides access to an UART on a DHCOM Module.
 *  The class does not provide protection from re-opening the same UART device many times.
 *  The class provides non-blocking IO, returning the number of symbols actually sent or received.
 *  \internal
 *  The class itself is a thin wrapper of OS-dependent implementation class.
 */


/** \fn UART::UART(const System &sys, UART::Device device)
 *  \brief Creates the UART object. Requires to know on which system we run (a fully set System object).
 */


/** \fn UART::~UART()
 *  \brief Destroys the UART object, closing it if necessary.
 */


/** \fn STATUS UART::setCommParams(UART::BaudRate baudRate, UART::PARITY parity, UART::StopBits stopBits, UART::FLOWCONTROL flowControl)
 *  \brief Sets the communication parameters for the UART.
 *  \param baudRate - the baud rate;
 *  \param parity - the parity setting;
 *  \param stopBits - 1 or 2 stop bits;
 *  \param flowControl - the flow control setting.
 *  \return - can return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_CONFIG_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS UART::open();
 *  \brief Opens the UART device for I/O.
 *  \return - can return STATUS_HARDWARE_UNDEFINED, STATUS_DEVICE_DOESNT_EXIST, STATUS_DEVICE_ALREADY_OPEN, STATUS_DEVICE_OPEN_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS UART::close();
 *  \brief Closes the UART device.
 *  \return STATUS_SUCCESS.
 */


/** \fn bool UART::isOpen() const;
 *  \brief Checks if the UART device is open.
 *  \return true if the device is open.
 */


/** \fn uint32_t UART::write(const uint8_t *buffer, uint32_t length, STATUS *status = NULL)
 *  \brief Writes an array of bytes (at least partially) to the UART.
 *  \param buffer - the source array
 *  \param length - the number of bytes to write
 *  \param status - if NULL, no status returned, otherwise may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_WRITE_FAILED or STATUS_SUCCESS.
 *  \return - the number of bytes actually written (0 and up to length)
 * The function is non-blocking, which means that if the UART write buffer is full, it will return with result less than the length.
 * It also may return with result of 0, which means that the buffer is still full.
 */


/** \fn uint32_t UART::read(uint8_t *buffer, uint32_t length, STATUS *status = NULL)
 *  \brief Reads an array of bytes (at least partially) from the UART.
 *  \param buffer - the destination array, must be at least of size length
 *  \param length - the number of bytes to read
 *  \param status - if NULL, no status returned, otherwise may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_READ_FAILED or STATUS_SUCCESS.
 *  \return - the number of bytes actually gotten (0 and up to length)
 * The function is non-blocking, which means that if the UART read buffer becomes empty, it will return with result less than length.
 * It also may return with result of 0, which means that the buffer is still empty.
 */


/** \fn STATUS UART::setPin(UART::OPIN pin, bool value);
 *  \brief Setting level of a pin.
 *  \param pin - the pin for which to set the level;
 *  \param value - true for high level, false for low level;
 *  \return may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_WRITE_FAILED or STATUS_SUCCESS;
 */


/** \fn bool UART::getPin(UART::IPIN pin, STATUS *status = NULL) const;
 *  \brief Getting status of a pin.
 *  \param pin - the pin for which to get the status;
 *  \param status - if NULL, no status returned, otherwise may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_READ_FAILED or STATUS_SUCCESS.
 *  \return true for high level on pin, false for low.
 */


} // namespace dhcom


#endif /* UART_H_ */
