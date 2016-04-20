/*
 * I2CSlave.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#include <hal/I2CSlave.h>


using namespace dhcom;


I2CSlave::I2CSlave(I2CBus *bus, uint16_t address)
: bus_(bus)
, address_(address)
{
}


STATUS I2CSlave::read(uint8_t *buffer, quint32 count) const
{
	STATUS st =	bus_->selectSlave(address_);
	if(st) return st;

	return bus_->read(buffer, count);
}


uint8_t I2CSlave::read(STATUS *status) const
{
	STATUS st =	bus_->selectSlave(address_);
	if(st)
	{
		if(status) *status = st;
		return 0;
	}
	else
	{
		return bus_->read(status);
	}
}


STATUS I2CSlave::write(const uint8_t *buffer, quint32 count)
{
	STATUS st =	bus_->selectSlave(address_);
	if(st) return st;

	return bus_->write(buffer, count);
}


STATUS I2CSlave::write(uint8_t byte)
{
	STATUS st = bus_->selectSlave(address_);
	if(st) return st;

	return bus_->write(byte);
}
