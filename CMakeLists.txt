project(DHCOM_HAL)
cmake_minimum_required(VERSION 2.8)

include(CPack)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter Kishalov <pkishalov@dh-electronics.de>" CACHE STRING "Maintainer of the Debian Package")
set(CPACK_DEBIAN_PACKAGE_VERSION "0.1" CACHE STRING "Version of the Debian Package")


set(HW 'DHCOM_iMX6' CACHE STRING "HW to build for: DHCOM_AM35|DHCOM_AM33|DHCOM_iMX25|DHCOM_iMX6|DHCOM_iMX6_rev200|DHCOM_iMX6_rev300|x86")
set(OS 'linux-arm' CACHE STRING "OS to build for: linux-arm|linux|win32")


include_directories(
    include
)


if(${OS} STREQUAL 'win32')
    add_definitions(-DDHCOM_HAL=__declspec(dllexport))
    set(IMPL_DIR Win32)
else()
    set(IMPL_DIR Linux)
endif()


if(${HW} STREQUAL 'x86')
    add_definitions(-DUSE_HARDWARE_PC)
elseif(${HW} STREQUAL 'DHCOM_AM35')
    add_definitions(-DUSE_HARDWARE_DHCOM_AM35)
elseif(${HW} STREQUAL 'DHCOM_AM33')
    add_definitions(-DUSE_HARDWARE_DHCOM_AM33)
elseif(${HW} STREQUAL 'DHCOM_iMX25')
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX25)
elseif(${HW} STREQUAL 'DHCOM_iMX6_rev200')
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6_REV200)
elseif(${HW} STREQUAL 'DHCOM_iMX6_rev300')
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6_REV300)
elseif(${HW} STREQUAL 'DHCOM_iMX6')
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6)
endif()


list(APPEND SOURCES
    src/${IMPL_DIR}/System.cpp
    src/${IMPL_DIR}/UART.cpp
    include/DHCOM_HAL.h
    include/hal/Types.h
    include/hal/System.h
    include/hal/UART.h
)


if(NOT ${HW} STREQUAL 'x86')
    list(APPEND SOURCES
        src/I2CSlave.cpp
        src/${IMPL_DIR}/GPIO.cpp
        src/${IMPL_DIR}/I2CBus.cpp
        src/${IMPL_DIR}/PhysMem.cpp
        src/${IMPL_DIR}/SPI.cpp
        src/${IMPL_DIR}/ADC.cpp
        include/hal/GPIO.h
        include/hal/I2CBus.h
        include/hal/I2CSlave.h
        include/hal/PhysMem.h
        include/hal/SPI.h
        include/hal/ADC.h
    )
endif()


add_library(DHCOM_HAL SHARED ${SOURCES})
install_targets(/lib ${PROJECT_NAME})

