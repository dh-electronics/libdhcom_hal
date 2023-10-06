// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2012 DH electronics GmbH
/*
 *  Created on: Dec 19, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef I2CSLAVE_H_
#define I2CSLAVE_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>
#include <hal/I2CBus.h>


namespace dhcom
{


class DHCOM_HAL I2CSlave
{
public:
	I2CSlave(I2CBus *bus, uint16_t address);
	I2CSlave(const I2CSlave &) = delete;

    STATUS 	read(uint8_t *buffer, uint32_t count) const;
	uint8_t	read(STATUS *status = NULL) const;
    STATUS 	write(const uint8_t *buffer, uint32_t count);
	STATUS 	write(uint8_t byte);

private:
	I2CBus * const 	bus_;
	uint16_t 	address_;
};


/** \file
 *  \brief Declaration of a I2CSlave class.
 *
 *  \class I2CSlave
 *  \brief Class representing a slave device of a I2C bus.
 *  This is a convenience class, which simplifies operations with I2C slave devices.
 *  It automates the selection of I2C slave in case when several slaves are operating on the same bus.
 */


/** \fn I2CSlave::I2CSlave(I2CBus *bus, uint16_t address);
 *  \brief Create an I2CSlave object and remember its properties (address and the bus).
 *  \param bus - a pointer to the I2CBus object, must not be NULL.
 *  \param address - the address of the slave on the bus.
 */


/** \fn STATUS I2CSlave::read(uint8_t *buffer, uint32_t count) const;
 *  \brief Reads a block of bytes from the I2C slave device.
 *  \param buffer - a pointer to the buffer to receive the incoming data.
 *  \param count - number of bytes to receive.
 *  \return - may return same values as I2CBus::selectSlave() and I2CBus::read().
 */


/** \fn uint8_t I2CSlave::read(STATUS *status = NULL) const;
 *  \brief Read one byte from the I2C slave device.
 *  \param status - if NULL - no status is returned, otherwise status is assigned from same set of values as for I2CBus::selectSlave() and I2CBus::read().
 *  \return The value of the received byte.
 */


/** \fn STATUS I2CSlave::write(const uint8_t *buffer, uint32_t count);
 *  \brief Write a block of bytes to the I2C slave device.
 *  \param buffer - a pointer to the buffer from which to send the data;
 *  \param count - number of bytes to send.
 *  \return - may return same values as I2CBus::selectSlave() and I2CBus::write().
 */


/** \fn STATUS I2CSlave::write(uint8_t byte);
 *  \brief Write one byte to the I2C slave device.
 *  \param byte - the value to send.
 *  \return - may return same values as I2CBus::selectSlave() and I2CBus::write().
 */


}


#endif /* I2CSLAVE_H_ */
