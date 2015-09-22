#if defined(USE_HARDWARE_DHCOM_IMX6_REV200) || defined(USE_HARDWARE_DHCOM_IMX6_REV300) || defined(USE_HARDWARE_DHCOM_IMX6)
// UARTS are common to the both revisions
static const char *IMX6_UARTS[] = {
    "/dev/ttymxc0", "/dev/ttymxc4", "/dev/ttymxc3", };

static const char *IMX6_SPI_BUS[] = {
    "/dev/spidev0.2", "/dev/spidev1.0" };
#endif


#if defined(USE_HARDWARE_DHCOM_IMX6_REV200)
#   warning "Including HARDWARE_DHCOM_IMX6_REV200"

static const uint8_t IMX6_REV200_GPIOpins[] = { 2, 4, 5, 163, 101, 116, 91, 103, 104, 174, 175, 105, 192, 193, 149, 205, 18, 16, 17, 19, 20, 146, 147 };

static const char *IMX6_REV200_I2C_BUS[] = {
        "/dev/i2c-0", "/dev/i2c-1", "/dev/i2c-2", };

static HardwareProps IMX6_REV200_Props_struct = {
    System::HARDWARE_DHCOM_IMX6_REV200,
    sizeof(IMX6_REV200_GPIOpins), IMX6_REV200_GPIOpins,
    3, IMX6_UARTS,
    3, IMX6_REV200_I2C_BUS,
    2, IMX6_SPI_BUS};

#	define IMX6_REV200_Props (&IMX6_REV200_Props_struct)

#else
#	define IMX6_REV200_Props NULL
#endif


#if defined(USE_HARDWARE_DHCOM_IMX6_REV300) || defined(USE_HARDWARE_DHCOM_IMX6)
#   warning "Including HARDWARE_DHCOM_IMX6_REV300"

static const uint8_t IMX6_REV300_GPIOpins[] = { 2, 4, 5, 163, 101, 116, 91, 103, 104, 174, 175, 105, 192, 193, 149, 205, 18, 16, 17, 19, 20, 146, 147 };

static const char *IMX6_REV300_I2C_BUS[] = {
    "/dev/i2c-0", "/dev/i2c-1", "/dev/i2c-2", };

static HardwareProps IMX6_REV300_Props_struct = {
    System::HARDWARE_DHCOM_IMX6_REV300,
    sizeof(IMX6_REV300_GPIOpins), IMX6_REV300_GPIOpins,
    3, IMX6_UARTS,
    3, IMX6_REV300_I2C_BUS,
    2, IMX6_SPI_BUS };

#	define IMX6_REV300_Props (&IMX6_REV300_Props_struct)

#else
#	define IMX6_REV300_Props NULL
#endif
