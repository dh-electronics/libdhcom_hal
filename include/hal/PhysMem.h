// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: (c) 2012 DH electronics GmbH
/*
 *  Created on: 07.11.2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef PHYSMEM_H_
#define PHYSMEM_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>
#include <sys/types.h>


namespace dhcom
{


class PhysMemImpl;


class DHCOM_HAL PhysMem
{
public:
	PhysMem(uint32_t physAddress, uint32_t length);
    PhysMem(const char *devName, uint32_t physAddress, uint32_t length);
	~PhysMem();

	STATUS open();
    STATUS open(off_t pageOffset, size_t pageSize);
	STATUS close();
	bool isOpen() const;

	void * 			getAddress() const;
	void * 			getAddress(uint32_t offset) const { return static_cast <uint8_t *> (getAddress()) + offset; }
	uint32_t 		getLength() const;

private:
    PhysMem(const PhysMem &) : impl_(NULL) {}

	PhysMemImpl * const impl_;
};


/** \file
 *  \brief Definitions of class PhysMem for access to physical address space of the DHCOM module.
 *
 *  \class PhysMem
 *  \brief Represents a chunk of physical address space of DHCOM module.
 *
 *  Important: On Linux systems root privileges are required to map physical memory.
 */


/** \fn PhysMem::PhysMem(uint32_t physAddress, uint32_t length)
 *  \brief Creates the PhysMem object and remembers the related segment of the physical addresses.
 *  \param physAddress - start of the physical address space.
 *  \param length - the length of the address space in bytes.
 */


/** \fn PhysMem::~PhysMem()
 *  \brief Destroys an object of the class, closing it if necessary.
 */


/** \fn STATUS PhysMem::open();
 *  \brief Claims the physical address space.
 *  \return STATUS_DEVICE_ALREADY_OPEN, STATUS_DEVICE_OPEN_FAILED or STATUS_SUCCESS.
 */


/** \fn STATUS PhysMem::close();
 *  \brief Releases the physical address space.
 *  \return STATUS_DEVICE_CLOSE_FAILED or STATUS_SUCCESS.
 */


/** \fn bool PhysMem::isOpen() const;
 *  \brief Checks if the physical address space was opened and is accessible.
 *  \return true - the physical address space is accessible.
 */


/** \fn void *PhysMem::getAddress() const;
 *  \brief Returns the virtual address for access to the represented physical address space.
 *  \return - a void pointer to the mapped block of memory, through which the access to physical address space is possible. Returns NULL if was not opened.
 */


/** \fn uint32_t PhysMem::getLength() const;
 *  \brief Get the length of the physical address segment.
 *  \return the length in bytes.
 */


}


#endif /* PHYSMEM_H_ */
