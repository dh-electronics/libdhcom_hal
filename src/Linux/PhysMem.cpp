/*
 * PhysMem.cpp
 *
 *  Created on: Dec 19, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef __linux__
#error This file must be compiled for LINUX target platform. Check the project.
#endif


#include <hal/PhysMem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


namespace dhcom
{


class PhysMemImpl
{
private:
	PhysMemImpl(uint32_t physAddress, uint32_t length);
	~PhysMemImpl();

	inline STATUS open();
	inline STATUS close();
	bool isOpen() const 		{ return deviceHandle_ > 0; }

	void * 	getAddress() const 	{ return address_ + pageOffset_; }
	uint32_t 	getLength() const 	{ return length_; }

	const uint32_t 	physAddress_;
	const uint32_t 	length_;
	int 			deviceHandle_;
	uint8_t *		address_;
	uint32_t 		pageOffset_;

	friend class PhysMem;
};


// PhysMem::


PhysMem::PhysMem(uint32_t physAddress, uint32_t length)
: impl_(new PhysMemImpl(physAddress, length))
{
}


PhysMem::~PhysMem()
{
	delete impl_;
}


STATUS PhysMem::open()
{
	return impl_->open();
}


STATUS PhysMem::close()
{
	return impl_->close();
}


void * 	PhysMem::getAddress() const
{
	return impl_->getAddress();
}


uint32_t 	PhysMem::getLength() const
{
	return impl_->getLength();
}


bool PhysMem::isOpen() const
{
	return impl_->isOpen();
}



// PhysMemImpl::


PhysMemImpl::PhysMemImpl(uint32_t physAddress, uint32_t length)
: physAddress_(physAddress)
, length_(length)
, deviceHandle_(-1)
, address_(NULL)
{

}


PhysMemImpl::~PhysMemImpl()
{
	close();
}


STATUS PhysMemImpl::open()
{
	if(isOpen())
		return STATUS_DEVICE_ALREADY_OPEN;

	deviceHandle_ = ::open("/dev/mem", O_RDWR | O_SYNC);
	if(0 > deviceHandle_)
		return STATUS_DEVICE_OPEN_FAILED;

	pageOffset_ = physAddress_ % getpagesize();
	off_t pageStart = physAddress_ - pageOffset_;

	address_ = (uint8_t*)mmap(NULL, length_ + pageOffset_, PROT_READ | PROT_WRITE, MAP_SHARED, deviceHandle_, pageStart);

	// printf("\nphys %x virt %x page %x offset %x\n", physAddress_, address_, pageStart, pageOffset_);

	if(address_ == MAP_FAILED)
		return STATUS_DEVICE_OPEN_FAILED;

	return STATUS_SUCCESS;
}


STATUS PhysMemImpl::close()
{
	if(isOpen())
	{
		if(0 > munmap(address_, length_ + pageOffset_))
			return STATUS_DEVICE_CLOSE_FAILED;

		address_ = NULL;

		::close(deviceHandle_);
		deviceHandle_ = -1;
	}

	return STATUS_SUCCESS;
}


}
