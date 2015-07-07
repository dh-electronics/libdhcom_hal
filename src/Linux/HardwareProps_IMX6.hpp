#if defined(USE_HARDWARE_DHCOM_IMX6_REV200) || defined(USE_HARDWARE_DHCOM_IMX6_REV300) || defined(USE_HARDWARE_DHCOM_IMX6)
// UARTS are common to the both revisions
const char *IMX6_UARTS[] = {
        "/dev/ttymxc0", "/dev/ttymxc4", "/dev/ttymxc3", };
#endif


#if defined(USE_HARDWARE_DHCOM_IMX6_REV200)
#   warning "Including HARDWARE_DHCOM_IMX6_REV200"

const uint8_t IMX6_REV200_GPIOpins[] = { 2, 4, 5, 163, 101, 116, 91, 103, 104, 174, 175, 105, 192, 193, 149, 205, 18, 16, 17, 19, 20, 146, 147 };

const uint8_t IMX6_REV200_I2C_BUS[] = { 0, 1, 2 };

HardwareProps IMX6_REV200_Props_struct = {
        System::HARDWARE_DHCOM_IMX6_REV200, sizeof(IMX6_REV200_GPIOpins), IMX6_REV200_GPIOpins, 3, IMX6_UARTS, IMX6_REV200_I2C_BUS};

#	define IMX6_REV200_Props (&IMX6_REV200_Props_struct)

#else
#	define IMX6_REV200_Props NULL
#endif


#if defined(USE_HARDWARE_DHCOM_IMX6_REV300) || defined(USE_HARDWARE_DHCOM_IMX6)
#   warning "Including HARDWARE_DHCOM_IMX6_REV300"

const uint8_t IMX6_REV300_GPIOpins[] = { 2, 4, 5, 163, 101, 116, 91, 103, 104, 174, 175, 105, 192, 193, 149, 205, 18, 16, 17, 19, 20, 146, 147 };

const uint8_t IMX6_REV300_I2C_BUS[] = { 1, 0, 2 };

HardwareProps IMX6_REV300_Props_struct = {
        System::HARDWARE_DHCOM_IMX6_REV300, sizeof(IMX6_REV300_GPIOpins), IMX6_REV300_GPIOpins, 3, IMX6_UARTS, IMX6_REV300_I2C_BUS};

#	define IMX6_REV300_Props (&IMX6_REV300_Props_struct)

#else
#	define IMX6_REV300_Props NULL
#endif
