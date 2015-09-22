/*
 * HardwareProps_AM35.hpp
 *
 *  Created on: Jul 4, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef USE_HARDWARE_DHCOM_AM33
#	define AM33_Props NULL
#else

#   warning "Including HARDWARE_DHCOM_AM33"

static const uint8_t AM33_GPIOpins[] = {
    20, 115, 116, 65, 114, 112, 111, 7, 113, 117, 57, 56, 55, 54, 51, 50, 49, };

static const char *AM33_UARTS[] = {
    "/dev/ttyO0", "/dev/ttyO1", };

static const char *AM33_I2C_BUS[] = {
    "/dev/i2c-2", NULL, "/dev/i2c-0" };

static const char *AM33_SPI_BUS[] = {
    "/dev/spidev1.0", "/dev/spidev2.0" };

static HardwareProps AM33_Props_struct = {
    System::HARDWARE_DHCOM_AM33,
    17, AM33_GPIOpins,
    2, AM33_UARTS,
    2, AM33_I2C_BUS,
    2, AM33_SPI_BUS};

#	define AM33_Props (&AM33_Props_struct)
#endif
