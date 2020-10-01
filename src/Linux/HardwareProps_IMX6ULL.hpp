/*
 * HardwareProps_IMX6ULL.hpp
 *
 *  Created on: April 25, 2019
 *      Author: Ludwig Zenz (LZ), DH electronics GmbH
 */

#ifndef USE_HARDWARE_DHCOM_IMX6ULL
#	define IMX6ULL_Props NULL
#else

#   warning "Including HARDWARE_DHCOM_IMX6ULL"

static const uint8_t IMX6ULL_GPIOpins[] = {
    128, 129, 130, 131, 132, 135, 136, 137, 18, 116, 114, 113, 115, 124, 123, 122, 121, 120, 119, 118, 117 };
 /* A    B    C    D    E    F    G    H    I   J    K    L    M    N    O    P    Q    R    S    T    U*/

static const char *IMX6ULL_UARTS[] = {
    "/dev/ttymxc0", "/dev/ttymxc1" };

static const char *IMX6ULL_I2C_BUS[] = {
    "/dev/i2c-1", "/dev/i2c-0" };

static const char *IMX6ULL_SPI_BUS[] = {
    "/dev/spidev0.0", "/dev/spidev3.0" };

static HardwareProps IMX6ULL_Props_struct = {
    System::HARDWARE_DHCOM_IMX6ULL,
    sizeof(IMX6ULL_GPIOpins), IMX6ULL_GPIOpins,
    2, IMX6ULL_UARTS,
    2, IMX6ULL_I2C_BUS,
    2, IMX6ULL_SPI_BUS };

#	define IMX6ULL_Props (&IMX6ULL_Props_struct)
#endif
