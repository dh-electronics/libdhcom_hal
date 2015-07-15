/*
 * HardwareProps_PC.hpp
 *
 *  Created on: Jul 4, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef USE_HARDWARE_PC
#	define PC_Props NULL
#else

#  pragma message ("Including HARDWARE_PC for Win32")

const uint8_t PC_GPIOpins[] = {0};

const char *PC_UARTS[] = {
	"COM1", "COM2", "COM3", 
	"COM4", "COM5",	"COM6", 
	"COM7", "COM8", "COM9", 
	"\\\\.\\COM10",	"\\\\.\\COM11", 
	"\\\\.\\COM12",	"\\\\.\\COM13", 
	"\\\\.\\COM14", "\\\\.\\COM15",
	"\\\\.\\COM16", "\\\\.\\COM17", 
	"\\\\.\\COM18",	"\\\\.\\COM19", 
	"\\\\.\\COM20",};

HardwareProps PC_Props_struct = {
		System::HARDWARE_PC, 0, PC_GPIOpins, 20, PC_UARTS};

#	define PC_Props (&PC_Props_struct)
#endif

