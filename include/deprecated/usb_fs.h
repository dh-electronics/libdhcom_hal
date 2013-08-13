/*
 * usb_fs.h
 *
 *  Created on: 09.11.2012
 *      Author: devel
 */

#ifndef USB_FS_H_
#define USB_FS_H_



class UsbFs
{
public:
	enum Device
	{
		DEVICE_USB1,
		DEVICE_USB2,
		DEVICE_OTG
	};

	UsbFs(Device device);
	virtual ~UsbFs();

	virtual const char *getPath() const;
};


/** \file
 *  \brief Definitions of UsbFs class.
 *
 *  \class UsbFs
 *  \brief Abstraction of a usb storage device.
 *  The class performs necessary checks of that the required usb storage was detected and mounted, and is accessible.
 *  Then, it can provide a path string to the storage file system, which can be used for accessing files there.
 */

#endif /* USB_FS_H_ */
