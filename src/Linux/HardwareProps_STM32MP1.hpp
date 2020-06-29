/*
 * HardwareProps_STM32MP1.hpp
 *
 *  Created on: June 29, 2020
 *      Author: Ludwig Zenz (LZ), DH electronics GmbH
 */

#ifndef USE_HARDWARE_DHCOM_STM32MP1
#   define STM32MP1_Props NULL
#else

#   warning "Including HARDWARE_DHCOM_STM32MP1"

static const uint8_t STM32MP1_GPIOpins[] = { /* TODO: This are still i.MX6ULL gpio numbers, bigger effort required to use gpiochip interface */
    128, 129, 130, 131, 132, 135, 136, 137, 18, 116, 114, 113, 115, 124, 123, 122, 121, 120, 119, 118, 117 };
 /* A    B    C    D    E    F    G    H    I   J    K    L    M    N    O    P    Q    R    S    T    U */

static const char *STM32MP1_UARTS[] = {
    "/dev/ttySTM0", "/dev/ttySTM2", "/dev/ttySTM1"};

static const char *STM32MP1_I2C_BUS[] = {
    "/dev/i2c-1", "/dev/i2c-0", "/dev/i2c-2", };

static const char *STM32MP1_SPI_BUS[] = {
    "/dev/spidev0.0" };

static HardwareProps STM32MP1_Props_struct = {
    System::HARDWARE_DHCOM_STM32MP1,
    sizeof(STM32MP1_GPIOpins), STM32MP1_GPIOpins,
    3, STM32MP1_UARTS,
    3, STM32MP1_I2C_BUS,
    1, STM32MP1_SPI_BUS };

#   define STM32MP1_Props (&STM32MP1_Props_struct)
#endif
