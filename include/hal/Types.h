/*
 * Types.h
 *
 *  Created on: Jun 23, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef TYPES_H_
#define TYPES_H_

#ifdef __GNUC__

#	include <stdint.h>
#   include <stddef.h>

#elif defined(_MSC_VER) && (_MSC_VER > 1300)

typedef signed __int8     int8_t;
typedef signed __int16    int16_t;
typedef signed __int32    int32_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  quint32;

#else

typedef signed char       int8_t;
typedef signed short      int16_t;
typedef signed int        int32_t;
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned int      quint32;

#endif


namespace dhcom
{

/**
 * \brief Enumeration for common return codes from library class methods.
 *
 * Most of the class methods in the library return the STATUS code to reflect the success of the requested operation.
 * This enumeration will be extended as more HAL classes are done.
 */
enum STATUS
{
	// system responses
	STATUS_SUCCESS = 0,					///< Operation completed successfully
	STATUS_HARDWARE_UNDEFINED,			///< The target hardware was not defined, create the System object with the hardware type
	STATUS_HARDWARE_ALREADY_DEFINED,	///< The target hardware was already defined, no way to re-define
	STATUS_HARDWARE_UNSUPPORTED,		///< The requested target hardware is unsupported by this library built
	STATUS_DEVICE_DOESNT_EXIST,			///< The requested I/O device does not exist for this target hardware

	// common device responses
	STATUS_DEVICE_OPEN_FAILED,			///< opening I/O device failed
	STATUS_DEVICE_CLOSE_FAILED,			///< closing I/O failed
	STATUS_DEVICE_NOT_OPEN,				///< device must be open to perform the requested operation
	STATUS_DEVICE_ALREADY_OPEN,			///< trying to open the already open device
	STATUS_DEVICE_READ_FAILED,			///< reading from the device failed
	STATUS_DEVICE_WRITE_FAILED,			///< writing to the device failed
	STATUS_DEVICE_CONFIG_FAILED,		///< attempt to configure the device failed (parameters invalid)

	// i2c specific
	STATUS_I2C_SLAVE_SELECT_FAILED,		///< selecting i2c slave failed

	// gpio specific

	// spi specific

	// uart specific
};


/**
 * \file
 * \brief Definitions of linux-like fixed-size types for systems which do not support them. Definition of STATUS enumeration.
 */


}


#endif /* TYPES_H_ */
