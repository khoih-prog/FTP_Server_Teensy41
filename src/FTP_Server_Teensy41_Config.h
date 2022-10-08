/**********************************************************************************************************************
  FTP_Server_Teensy41.hpp

  FTP_Server_Teensy41 is an FTP Server for Teensy 4.1 using SD, FS, etc. with QNEthernet or NativeEthernet
  
  Based on and modified from Arduino-Ftp-Server Library (https://github.com/gallegojm/Arduino-Ftp-Server)
  Built by Khoi Hoang https://github.com/khoih-prog/FTP_Server_Teensy41
  
  Version: 1.2.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      30/04/2022 Initial porting and coding for Teensy 4.1 using built-in QNEthernet, NativeEthernet
  1.1.0   K Hoang      16/05/2022 Fix bug incomplete downloads from server to client
  1.2.0   K Hoang      24/05/2022 Add support to WiFiNINA, such as Adafruit Airlift Featherwing. 
                                  Configurable user_name length to 63 and user_password to 127
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
 **                         SETTINGS FOR FTP SERVER                            **
 **                                                                            **
 *******************************************************************************/

#pragma once

#ifndef FTP_SERVER_TEENSY41_CONFIG_H
#define FTP_SERVER_TEENSY41_CONFIG_H

////////////////////////////////////////////////////////////////////////////

// Files system
#define FTP_SDFAT1     0     // Library SdFat version 1.4.x
#define FTP_SDFAT2     1     // Library SdFat version >= 2.0.2
#define FTP_SPIFM      2     // Libraries Adafruit_SPIFlash and SdFat-Adafruit-Fork
#define FTP_FATFS      3     // Library FatFs


// Select one of the previous files system as default
#if !defined(FTP_FILESYST)
  #define FTP_FILESYST        FTP_SDFAT2
#endif

// Redirect debug info to console or other port
#define FTP_SERIAL Serial
// #define FTP_SERIAL SerialUSB

// Disconnect client after 5 minutes of inactivity (expressed in seconds)
#define FTP_TIME_OUT          (5 * 60)

// Wait for authentication for 10 seconds (expressed in seconds)
//#define FTP_AUTH_TIME_OUT 10
#define FTP_AUTH_TIME_OUT     60

// Size of file buffer for read/write
// Transfer speed depends of this value
// Best value depends on many factors: SD card, client side OS, ...
// But it can be reduced to 512 if memory usage is critical.
#if ( !defined(FTP_BUF_SIZE) || (FTP_BUF_SIZE < 2048) )
  #if(_FTP_SERVER_LOGLEVEL_>2)
    #warning Using default FTP_BUF_SIZE = 2048
  #endif
  
  #define FTP_BUF_SIZE        2048 //1024 // 512
#endif

#if !defined(PASV_RESPONSE_STYLE_NEW)
  // True  => "227 Entering Passive Mode (192,168,2,112,157,218)"
  // False => "227 Entering Passive Mode (4043483328, port 55600)"
  #define PASV_RESPONSE_STYLE_NEW       true
#endif

#endif // FTP_SERVER_TEENSY41_CONFIG_H
