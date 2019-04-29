project(DHCOM_HAL)
cmake_minimum_required(VERSION 2.8)

include(CPack)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Peter Kishalov <pkishalov@dh-electronics.de>" CACHE STRING "Maintainer of the Debian Package")
set(CPACK_DEBIAN_PACKAGE_VERSION "0.4" CACHE STRING "Version of the Debian Package")
set(CPACK_PACKAGE_NAME "dhcom-hal" CACHE STRING "Package name")


set(HW_DHCOM_iMX6 ON CACHE BOOL "Build for DHCOM_IMX6")
set(HW_DHCOM_iMX6_rev200 OFF CACHE BOOL "Build for DHCOM_iMX6_rev200")
set(HW_DHCOM_iMX6_rev300 OFF CACHE BOOL "Build for DHCOM_iMX6_rev300")
set(HW_DHCOM_iMX6ULL ON CACHE BOOL "Build for DHCOM_IMX6ULL")
set(HW_DHCOM_iMX25 ON CACHE BOOL "Build for DHCOM_iMX25")
set(HW_DHCOM_AM35 ON CACHE BOOL "Build for DHCOM_AM35")
set(HW_DHCOM_AM33 ON CACHE BOOL "Build for DHCOM_AM33")
set(HW_X86 OFF CACHE BOOL "Build for X86")

set(OS "linux-arm" CACHE STRING "OS to build for: linux-arm|linux|win32")
set_property(CACHE OS PROPERTY STRINGS
    linux-arm
    linux
    win32
)


if(${OS} STREQUAL "win32")
    add_definitions("-DDHCOM_HAL=__declspec(dllexport)")
    set(IMPL_DIR Win32)
else()
    set(IMPL_DIR Linux)
endif()


include_directories(include)


list(APPEND SOURCES
    src/${IMPL_DIR}/System.cpp
    src/${IMPL_DIR}/UART.cpp
    include/DHCOM_HAL.h
    include/hal/Types.h
    include/hal/System.h
    include/hal/UART.h
)


if(${HW_X86})
    add_definitions(-DUSE_HARDWARE_PC)
else()
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

if(${HW_DHCOM_AM35})
    add_definitions(-DUSE_HARDWARE_DHCOM_AM35)
endif()

if(${HW_DHCOM_AM33})
    add_definitions(-DUSE_HARDWARE_DHCOM_AM33)
endif()

if(${HW_DHCOM_iMX25})
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX25)
endif()

if(${HW_DHCOM_iMX6_rev200})
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6_REV200)
endif()

if(${HW_DHCOM_iMX6_rev300})
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6_REV300)
endif()

if(${HW_DHCOM_iMX6})
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6)
endif()

if(${HW_DHCOM_iMX6ULL})
    add_definitions(-DUSE_HARDWARE_DHCOM_IMX6ULL)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1)

# Expose DHCOM_HAL's public includes to other subprojects through cache variable
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

