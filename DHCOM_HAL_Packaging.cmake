dhcom_hal_cmake_hook(INIT_CPACK)
if(NOT EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  message(STATUS "CPack is not found. SKIP")
  return()
endif()

set(CPACK_set_DESTDIR "on")

if(NOT DHCOM_HAL_CUSTOM_PACKAGE_INFO)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "DHCOM Hardware Abstraction Library")
  set(CPACK_PACKAGE_DESCRIPTION
"The DHCOM Hardware Abstraction Library provides access to commen hardware interfaces. The aim is to
provide a interface that is useable on multiple DHCOM boards.")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "DH electronics GmbH <embedded@dh-electronics.de>" CACHE STRING "Maintainer of the Debian Package")
  set(CPACK_PACKAGE_VERSION "${DHCOM_HAL_VCSVERSION}")
endif(NOT DHCOM_HAL_CUSTOM_PACKAGE_INFO)

#arch
if(X86)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
elseif(X86_64)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
elseif(ARM)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
elseif(AARCH64)
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "arm64")
else()
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
endif()

message("Package cpu architecture: " ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})

if(CPACK_GENERATOR STREQUAL "DEB")
  set(DHCOM_HAL_PACKAGE_ARCH_SUFFIX ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
else()
  set(DHCOM_HAL_PACKAGE_ARCH_SUFFIX ${CMAKE_SYSTEM_PROCESSOR})
endif()

#deb options
set(CPACK_DEB_COMPONENT_INSTALL   TRUE)
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION  "libs")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.dh-electronics.com")

#display names
set(CPACK_COMPONENT_DEV_DISPLAY_NAME     "Development files")
set(CPACK_COMPONENT_LIB_DISPLAY_NAME     "Library and license")

#depencencies
set(CPACK_DEBIAN_LIB_PACKAGE_SHLIBDEPS TRUE)
set(CPACK_COMPONENT_LIB_REQUIRED TRUE)
set(CPACK_COMPONENT_DEV_DEPENDS lib)

set(CPACK_COMPONENT_LIB_CONFLICTS "${DHCOM_HAL_PACKAGE_NAME}")
set(CPACK_COMPONENT_LIB_PROVIDES  "${DHCOM_HAL_PACKAGE_NAME}")
set(CPACK_COMPONENT_LIB_REPLACES  "${DHCOM_HAL_PACKAGE_NAME}")

set(CPACK_COMPONENT_DEV_CONFLICTS "${DHCOM_HAL_PACKAGE_NAME}-dev")
set(CPACK_COMPONENT_DEV_PROVIDES  "${DHCOM_HAL_PACKAGE_NAME}-dev")
set(CPACK_COMPONENT_DEV_REPLACES  "${DHCOM_HAL_PACKAGE_NAME}-dev")

set(CPACK_COMPONENT_LIB_DESCRIPTION "DHCOM Hardware Abstraction Library")
set(CPACK_DEBIAN_COMPONENT_LIB_NAME "${DHCOM_HAL_PACKAGE_NAME}")
set(CPACK_DEBIAN_LIB_PACKAGE_NAME "${CPACK_DEBIAN_COMPONENT_LIB_NAME}")
set(CPACK_DEBIAN_LIB_FILE_NAME "${CPACK_DEBIAN_COMPONENT_LIB_NAME}_${DHCOM_HAL_VCSVERSION}_${DHCOM_HAL_PACKAGE_ARCH_SUFFIX}.deb")

set(CPACK_COMPONENT_DEV_DESCRIPTION "Development files for DHCOM Hardware Abstraction Library")
set(CPACK_DEBIAN_COMPONENT_DEV_NAME "${DHCOM_HAL_PACKAGE_NAME}-dev")
set(CPACK_DEBIAN_DEV_PACKAGE_NAME "${CPACK_DEBIAN_COMPONENT_DEV_NAME}")
set(CPACK_DEBIAN_DEV_FILE_NAME "${DHCOM_HAL_PACKAGE_NAME}-dev_${DHCOM_HAL_VCSVERSION}_${DHCOM_HAL_PACKAGE_ARCH_SUFFIX}.deb")
set(CPACK_DEBIAN_DEV_PACKAGE_DEPENDS "${CPACK_DEBIAN_LIB_PACKAGE_NAME} (=> ${CPACK_PACKAGE_VERSION})")

set(CPACK_COMPONENTS_ALL lib dev)

dhcom_hal_cmake_hook(PRE_CPACK)
include(CPack)
dhcom_hal_cmake_hook(POST_CPACK)
