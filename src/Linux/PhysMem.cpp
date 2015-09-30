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
#include <sys/stat.h>
#include <fcntl.h>


namespace dhcom
{


class PhysMemImpl
{
private:
    PhysMemImpl(const char *devName, uint32_t physAddress, uint32_t length);
	~PhysMemImpl();

    inline STATUS open();
    inline STATUS open(off_t pageOffset, size_t pageSize); //< ignores the length and uses the exact page offset and size
	inline STATUS close();
    bool isOpen() const             { return deviceHandle_ > 0; }

    void * 	getAddress() const      { return address_ + pageOffset_; }
	uint32_t 	getLength() const 	{ return length_; }

    const char *    devName_;
	const uint32_t 	physAddress_;
	const uint32_t 	length_;
	int 			deviceHandle_;
	uint8_t *		address_;
    uint32_t 		pageOffset_;

	friend class PhysMem;
};


// PhysMem::


PhysMem::PhysMem(uint32_t physAddress, uint32_t length)
: impl_(new PhysMemImpl("/dev/mem", physAddress, length))
{
}


PhysMem::PhysMem(const char *devName, uint32_t physAddress, uint32_t length)
: impl_(new PhysMemImpl(devName, physAddress, length))
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


STATUS PhysMem::open(off_t pageOffset, size_t pageSize)
{
    return impl_->open(pageOffset, pageSize);
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


PhysMemImpl::PhysMemImpl(const char *devName, uint32_t physAddress, uint32_t length)
: devName_(devName)
, physAddress_(physAddress)
, length_(length)
, deviceHandle_(-1)
, address_(NULL)
, pageOffset_(0)
{

}


PhysMemImpl::~PhysMemImpl()
{
	close();
}


STATUS PhysMemImpl::open(off_t pageOffset, size_t pageSize)
{
	if(isOpen())
        return STATUS_SUCCESS;

    deviceHandle_ = ::open(devName_, O_RDWR | O_SYNC);
	if(0 > deviceHandle_)
		return STATUS_DEVICE_OPEN_FAILED;

    address_ = (uint8_t*)mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, deviceHandle_, pageOffset);

    // printf("\nphys %x virt %x", physAddress_, address_);

	if(address_ == MAP_FAILED)
		return STATUS_DEVICE_OPEN_FAILED;

	return STATUS_SUCCESS;
}


STATUS PhysMemImpl::open()
{
    pageOffset_ = physAddress_ % getpagesize();
    return open(physAddress_ - pageOffset_, length_ + pageOffset_);
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
