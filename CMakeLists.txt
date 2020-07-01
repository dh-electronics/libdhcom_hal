cmake_minimum_required(VERSION 3.0)
project(DHCOM_HAL)

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


list(APPEND SOURCES
    src/${IMPL_DIR}/System.cpp
    src/${IMPL_DIR}/UART.cpp
)

list(APPEND INCLUDES
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
    )
    list(APPEND INCLUDES
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


add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${DHCOM_HAL_SOURCE_DIR}/include>
)

include(GNUInstallDirs)

set(RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/copyright")
set(RESOURCE_FILE_CHANGELOG "${CMAKE_SOURCE_DIR}/changelog")

install(TARGETS ${PROJECT_NAME}
    EXPORT DHCOM_HAL
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
    COMPONENT lib
)

install(FILES ${RESOURCE_FILE_LICENSE} ${RESOURCE_FILE_CHANGELOG}
    DESTINATION /usr/share/doc/libdhcom-hal/
    COMPONENT lib
)

install(DIRECTORY include/ 
    DESTINATION include
    COMPONENT dev
    PATTERN "*.h" 
)

install(FILES ${RESOURCE_FILE_LICENSE} ${RESOURCE_FILE_CHANGELOG}
    DESTINATION /usr/share/doc/libdhcom-hal-dev/
    COMPONENT dev
)

include(DHCOM_HAL_Utils.cmake)

# ----------------------------------------------------------------------------
#  Autodetect if we are in a GIT repository
# ----------------------------------------------------------------------------
find_host_package(Git QUIET)

if(NOT DEFINED DHCOM_HAL_VCSVERSION AND GIT_FOUND)
  git_describe(DHCOM_HAL_VCSVERSION "${DHCOM_HAL_SOURCE_DIR}")
elseif(NOT DEFINED DHCOM_HAL_VCSVERSION)
  # We don't have git:
  set(DHCOM_HAL_VCSVERSION "unknown")
endif()
if(DHCOM_HAL_VCSVERSION)
  message("")
  message(${PROJECT_NAME} " vcs version: " ${DHCOM_HAL_VCSVERSION})
endif()

set(CPACK_GENERATOR "DEB")
set(DHCOM_HAL_PACKAGE_NAME "libdhcom-hal")
include(DHCOM_HAL_Packaging.cmake)

