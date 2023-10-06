# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: (c) 2012 DH electronics GmbH

project(dhcom-hal-tests)
cmake_minimum_required (VERSION 3.0)

list(APPEND SOURCES
    main.cpp
    test.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    dhcom-hal
)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11)

install(TARGETS ${PROJECT_NAME}
    DESTINATION bin
    COMPONENT lib
)

