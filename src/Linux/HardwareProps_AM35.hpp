/*
 * HardwareProps_AM35.hpp
 *
 *  Created on: Jul 4, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef USE_HARDWARE_DHCOM_AM35
#	define AM35_Props NULL
#else

#   warning Including HARDWARE_DHCOM_AM35

static const uint8_t AM35_GPIOpins[] = {
		116, 117, 118, 119, 125, 140, 141, 155, 152, 96, 94, 58, 97, 98, 95, 106, 105, 104, 103, 102, 101, 100, 99};

static const char *AM35_UARTS[] = {
		"/dev/ttyO0", "/dev/ttyO2", "/dev/ttyO1", };

static HardwareProps AM35_Props_struct = {
		System::HARDWARE_DHCOM_AM35, 23, AM35_GPIOpins, 3, AM35_UARTS };

#	define AM35_Props (&AM35_Props_struct)
#endif
