TARGET = DHCOM_HAL
TEMPLATE = lib
CONFIG -= qt 
CONFIG += dll

# Defining build type dependent settings
CONFIG(debug, debug|release) {
    message("Configuring DEBUG build.")
}
CONFIG(release, debug|release) {
    message("Configuring RELEASE build.")
}

# Defining platform-dependent settings
win32 {
	DEFINES += $$TARGET=__declspec(dllexport)
	!isEmpty(CE_SDK) {
		DHCOM_HAL_IMPL = WinCE
	} else {
		DHCOM_HAL_IMPL = Win32
	}
}

unix {
	DHCOM_HAL_IMPL = Linux
        BUILD=
}

# Includes
INCLUDEPATH += include
message("Includes: " $$INCLUDEPATH)

# All classes are available only on DHCOM modules
DHCOM_AM35 | DHCOM_iMX25 {
	SOURCES = \
	src/I2CSlave.cpp \
	src/$$DHCOM_HAL_IMPL/GPIO.cpp \
	src/$$DHCOM_HAL_IMPL/I2CBus.cpp \
	src/$$DHCOM_HAL_IMPL/PhysMem.cpp \
	src/$$DHCOM_HAL_IMPL/SPI.cpp \
	src/$$DHCOM_HAL_IMPL/ADC.cpp
	
	HEADERS = \
	include/hal/GPIO.h \
	include/hal/I2CBus.h \
	include/hal/I2CSlave.h \
	include/hal/PhysMem.h \
	include/hal/SPI.h \
	include/hal/ADC.h

        debian_package_path = /tmp/dhcom-hal
        debian.path = $$debian_package_path
        debian.commands = \
        svn export --force $$PWD/DEBIAN_PACKAGE $$debian_package_path ;\
        mkdir -p $$debian_package_path/usr/lib ;\
        cd $$OUT_PWD ;\
        cp -d lib* $$debian_package_path/usr/lib ;\
        dpkg -b $$debian_package_path dhcom-hal.deb ;\
        rm -rf $$debian_package_path

INSTALLS = debian
}

DHCOM_iMX25 {
	HEADERS += src/$$DHCOM_HAL_IMPL/imx_adc.h
        DEFINES += USE_HARDWARE_DHCOM_IMX25
}

DHCOM_iMX6: DEFINES += USE_HARDWARE_DHCOM_IMX6
DHCOM_iMX6_rev200: DEFINES += USE_HARDWARE_DHCOM_IMX6_REV200
DHCOM_iMX6_rev300: DEFINES += USE_HARDWARE_DHCOM_IMX6_REV300
DHCOM_AM35:DEFINES += USE_HARDWARE_DHCOM_AM35
x86:DEFINES += USE_HARDWARE_PC


# UART and System are available on all platforms
SOURCES += \
src/$$DHCOM_HAL_IMPL/System.cpp \
src/$$DHCOM_HAL_IMPL/UART.cpp

HEADERS += \
include/DHCOM_HAL.h \
include/hal/Types.h \
include/hal/System.h \
include/hal/UART.h

OTHER_FILES = \
DEBIAN_PACKAGE/DEBIAN/changelog \
DEBIAN_PACKAGE/DEBIAN/control \
DEBIAN_PACKAGE/DEBIAN/postinst \
DEBIAN_PACKAGE/DEBIAN/preinst


