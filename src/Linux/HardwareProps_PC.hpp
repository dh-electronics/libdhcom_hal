/*
 * HardwareProps_PC.hpp
 *
 *  Created on: Jul 4, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef USE_HARDWARE_PC
#	define PC_Props NULL
#else

#   warning "Including HARDWARE_PC"

const uint8_t PC_GPIOpins[] = {};

const char *PC_UARTS[] = {
		"/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3",
		"/dev/ttyS4", "/dev/ttyS5", "/dev/ttyS6", "/dev/ttyS7",
		"/dev/ttyS8", "/dev/ttyS9", "/dev/ttyS10", "/dev/ttyS11",
		"/dev/ttyS12", "/dev/ttyS13", "/dev/ttyS14", "/dev/ttyS15",
		"/dev/ttyS16", "/dev/ttyS17", "/dev/ttyS18", "/dev/ttyS19", };

HardwareProps PC_Props_struct = {
    System::HARDWARE_PC,
    0, PC_GPIOpins,
    20, PC_UARTS,
    0, NULL,
    0, NULL };

#	define PC_Props (&PC_Props_struct)
#endif

