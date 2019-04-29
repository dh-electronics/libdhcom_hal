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
    160, 161, 162, 163, 164, 167, 168, 169, 50, 148, 146, 145, 147, 156, 155, 154, 153, 152, 151, 150, 149 };
 /* A    B    C    D    E    F    G    H    I   J    K    L    M    N    O    P    Q    R    S    T    U*/

static const char *IMX6ULL_UARTS[] = {
    "/dev/ttymxc0", "/dev/ttymxc1" };

static const char *IMX6ULL_I2C_BUS[] = {
    "/dev/i2c-1", "/dev/i2c-0" };

static const char *IMX6ULL_SPI_BUS[] = {
    "/dev/spidev0.0" };

static HardwareProps IMX6ULL_Props_struct = {
    System::HARDWARE_DHCOM_IMX6ULL,
    sizeof(IMX6ULL_GPIOpins), IMX6ULL_GPIOpins,
    2, IMX6ULL_UARTS,
    2, IMX6ULL_I2C_BUS,
    1, IMX6ULL_SPI_BUS };

#	define IMX6ULL_Props (&IMX6ULL_Props_struct)
#endif
