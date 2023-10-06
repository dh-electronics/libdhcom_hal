// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: © 2012 DH electronics GmbH
/*
 *  Created on: Dec 17, 2012
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */


#ifndef DHCOM_HAL
#	ifdef __linux__
#		define DHCOM_HAL
#	else
#		define DHCOM_HAL __declspec(dllexport)
#	endif
#endif


/** \file
 *  \brief Exports definitions.
 *
 *  <b>DHCOM_HAL</b> macro is preceding every public class declaration in the library.
 *
 *  For manual compiling of the library on Windows - do the <b>/D "DHCOM_HAL=__declspec(dllexport)"</b> in compiler command line.
 **/


/** \mainpage DHCOM HAL - hardware abstraction C++ class library for DHCOM modules.
 * DHCOM HAL class library is used to abstract application code from the underlying OS - and hardware - specific device drivers.
 * \n The library can be compiled to support several systems with similar executable binary formats (OS/CPU architectures).
 * \n In its current state the library supports:
 * \li UART, SPI, I2C, Physical Memory and GPIO for DHCOM Linux targets
 * \li UART for PC Linux targets
 * \li UART for PC Windows targets
 *
 * \section compile_sec Compiling
 * It is intended to compile the DHCOM HAL with Qt4 build system (qmake), though it does not use any of the Qt libraries. 
 * \n The project file for building is supplied with the library (DHCOM_HAL.pro).
 * \n To compile for DHCOM Modules use the Qt for DHCOM targets installation, supplied in the Debian building environment by DH Electronics.
 * \n Use options CONFIG+=DHCOM_AM35 or CONFIG+=DHCOM_iMX25 to compile support for DHCOM_AM35 or DHCOM_iMX25 modules, or both of them.
 *
 * \subsection win_pc_sec Configuring for Windows pc
 * If you want to configure for Windows PC, execute the following command from the Visual Studio command prompt:
 * \n <b> qmake -spec win32-msvc2008 -tp vc CONFIG=debug_and_release</b>
 * \n The MS Visual studio project will be created. 
 *
 * \subsection wince_dhcom_sec Configuring for DHCOM Module running Windows CE6 or CE7
 * Under construction... 
 *
 * \subsection linux_pc_sec Configuring for Linux PC 
 * You can configure the build for linux host the following way:
 * \n <b> qmake -spec linux-g++-32 CONFIG=debug_and_release</b>
 * \n The makefile project will be created.
 *
 * \subsection linux_dhcom_sec Configuring for DHCOM module running Linux.
 * You can configure the build for DHCOM modules running linux the following way:
 * \n <b> qmake -spec linux-arm-gnueabi-g++ CONFIG=debug_and_release</b>
 * \n The mkspec linux-arm-gnueabi-g++ must be in place to do this.
 * \n The makefile project will be created.
 *
 * \subsection manual_sec Manual configuration
 * It is always possible to configure the library for compilation without qmake. 
 * \n To do this on Windows:
 * \li In MSVC2008 create a smart device project, selecting the correct SDK
 * \li add all header files from <b>include</b> and <b>include/hal</b> into the project
 * \li add appropriate source files from the <b>src</b>
 * \li observe correctness of definitions of supported targets in System.h
 * \li define DHCOM_HAL macro as <b>/D "DHCOM_HAL=__declspec(dllexport)"</b>
 * \n
 * \n To manually compile on linux:
 * \li select the appropriate source files from src/Linux
 * \li include the \b include directory
 * \li observe correctness of definitions of supported targets in System.h (USE_HARDWARE_...)
 * \li create a Makefile or Eclipse project
 *
 * \section license_sec License
 * This library source code is distributed under terms of BSD License:
 * \n <c><b> Copyright (c) 2013, DH Electronics GmbH. All rights reserved.</b>
 * \n Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * \li Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * \li Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * \li Neither the name of DH Electronics GmbH nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.
 * \n
 * \n <b> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</b>
 * </c>
 * 
 **/
