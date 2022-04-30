/**********************************************************************************************************************
  FTP_Server_Teensy41.h
   
  FTP_Server_Teensy41 is an FTP Server for Teensy 4.1 using SD, FS, etc. with QNEthernet or NativeEthernet
  
  Based on and modified from Arduino-Ftp-Server Library (https://github.com/gallegojm/Arduino-Ftp-Server)
  Built by Khoi Hoang https://github.com/khoih-prog/FTP_Server_Teensy41
  
  Version: 1.0.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      30/04/2022 Initial porting and coding for Teensy 4.1 using built-in QNEthernet, NativeEthernet
 ***********************************************************************************************************************/

/*
   FTP Serveur for Arduino Due, Arduino MKR
   and Ethernet shield W5100, W5200 or W5500
   ( or for Esp8266 with external SD card or SpiFfs ) *
   Copyright (c) 2014-2020 by Jean-Michel Gallego

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*******************************************************************************
 **                                                                            **
 **                       DEFINITIONS FOR FTP SERVER                           **
 **                                                                            **
 *******************************************************************************/
 
#pragma once

#ifndef FTP_SERVER_TEENSY41_H
#define FTP_SERVER_TEENSY41_H

////////////////////////////////////////////////////////////////////////////

#if ( defined(CORE_TEENSY) && defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41) )
  // For Teensy 4.1
  #if !defined(BOARD_NAME)
    #define BOARD_NAME              "TEENSY 4.1"
  #endif
  
  // Default to QNEthernet if nothing pre-selected
  #if !(USE_QN_ETHERNET || USE_NATIVE_ETHERNET || USE_ETHERNET_GENERIC)
    #define USE_QN_ETHERNET         true
    #define USE_NATIVE_ETHERNET     false
    #define USE_ETHERNET_GENERIC    false
  #endif
#else
  //#error Only Teensy 4.1 supported
#endif

////////////////////////////////////////////////////////////////////////////

#define FTP_SERVER_TEENSY41_VERSION            "FTP_Server_Teensy41 v1.0.0"

#define FTP_SERVER_TEENSY41_VERSION_MAJOR      1
#define FTP_SERVER_TEENSY41_VERSION_MINOR      0
#define FTP_SERVER_TEENSY41_VERSION_PATCH      0

#define FTP_SERVER_TEENSY41_VERSION_INT        1000000

////////////////////////////////////////////////////////////////////////////

#include "FTP_Server_Teensy41_Config.h"
#include "FTP_Server_Teensy41.hpp"
#include "FTP_Server_Teensy41_Impl.h"

#endif // FTP_SERVER_TEENSY41_H
