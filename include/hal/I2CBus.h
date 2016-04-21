/*
 * i2c.h
 *
 *  Created on: 07.11.2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef I2CBUS_H_
#define I2CBUS_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>


namespace dhcom
{


class System;
class I2CBusImpl;


class DHCOM_HAL I2CBus
{
public:
	/// Master device enumeration
	enum BUS
	{
        BUS_I2C1 = 0,	///< I2C master device 1
        BUS_I2C2,       ///< I2C master device 2
        BUS_I2C3        ///< I2C master device 3 (onboard)
	};

    I2CBus(const System &sys, BUS bus);
    explicit I2CBus(BUS bus);           // deprecated, used for compatibility with older versions
	~I2CBus();

	STATUS open();
	STATUS close();
	bool isOpen() const;

	STATUS selectSlave(uint16_t address);
    STATUS read(uint8_t *buffer, uint32_t count) const;
	uint8_t read(STATUS *status = NULL) const;
    STATUS write(const uint8_t *buffer, uint32_t count);
	STATUS write(uint8_t byte);

private:
    I2CBus(const I2CBus &) : impl_(NULL) {}

	I2CBusImpl * const impl_;
};


/** \file
 *  \brief Definitions of I2CBus class.
 *
 *  \class I2CBus
 *  \brief Represents an I2C bus of a DHCOM Module. Not available on PC.
 */


/** \fn I2CBus::I2CBus(I2C::BUS bus);
 *  \brief Constructs the I2CBus object.
 *  \param bus - the physical I2C bus to open later.
 */


/** \fn I2CBus::~I2CBus()
 *  \brief Destroys the I2CBus object and releases the resources.
 *
 *  This call also closes the bus if necessary.
 */


/** \fn STATUS I2CBus::open();
 *  \brief Opens the bus, claims the hardware resources.
 *  \return - may return STATUS_DEVICE_ALREADY_OPEN, STATUS_DEVICE_OPEN_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS I2CBus::close();
 *  \brief Closes the bus, releases resources.
 *  \return - always STATUS_SUCCESS.
 */


/** \fn bool I2CBus::isOpen() const;
 *  \brief Checks if the bus is open and ready for I/O.
 *  \return true if the bus is open.
 */


/** \fn STATUS I2CBus::selectSlave(uint16_t address);
 *  \brief Selects an I2C slave to communicate with.
 *  \param address - the slave address.
 *  \return - may return STATUS_DEVICE_NOT_OPEN, STATUS_I2C_SLAVE_SELECT_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS I2CBus::read(uint8_t *buffer, uint32_t count) const;
 *  \brief Reads a block of bytes from a slave.
 *  \param buffer - the pointer to receiving buffer, may not be NULL;
 *  \param count - the number of bytes to receive;
 *  \return - may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_READ_FAILED or STATUS_SUCCESS.
 */


/** \fn uint8_t I2CBus::read(STATUS *status = NULL) const;
 *  \brief Reads one byte from a slave.
 *  \param status - if NULL - no status returned, otherwise the status is assigned to  STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_READ_FAILED or STATUS_SUCCESS.
 *  \return - the value received.
 */


/** \fn STATUS I2CBus::write(const uint8_t *buffer, uint32_t count);
 *  \brief Writes a block of bytes to the slave.
 *  \param buffer - the pointer to buffer with the data, may not be NULL;
 *  \param count - the number of bytes to send;
 *  \return - may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_WRITE_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS I2CBus::write(uint8_t byte);
 *  \brief Writes one byte to the I2C slave.
 *  \param byte - the value to write.
 *  \return - may return STATUS_DEVICE_NOT_OPEN, STATUS_DEVICE_WRITE_FAILED or STATUS_SUCCESS.
 */


}


#endif /* I2CBUS_H_ */
