/*
 * HardwareProps_IMX25.hpp
 *
 *  Created on: Jul 4, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef USE_HARDWARE_DHCOM_IMX25
#	define IMX25_Props NULL
#else

#   warning "Including HARDWARE_DHCOM_IMX25"

const uint8_t IMX25_GPIOpins[] = {
		44, 85, 36, 37, 38, 39, 40, 41, 81, 10, 11, 8, 9, 117, 7, 6, 31, 30, 29, 28, 27};

const char *IMX25_UARTS[] = {
		"/dev/ttymxc1", "/dev/ttymxc2", "/dev/ttymxc4", };

const uint8_t IMX25_I2C_BUS[] = {0, 1, 2};

HardwareProps IMX25_Props_struct = {
        System::HARDWARE_DHCOM_IMX25, 21, IMX25_GPIOpins, 3, IMX25_UARTS, IMX25_I2C_BUS};

#	define IMX25_Props (&IMX25_Props_struct)
#endif
