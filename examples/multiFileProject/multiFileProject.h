/****************************************************************************************************************************
  multiFileProject.h
  FTP_Server_Teensy41 is an FTP Server for Teensy 4.1 using SD, FS, etc. with QNEthernet or NativeEthernet

  Based on and modified from Arduino-Ftp-Server Library (https://github.com/gallegojm/Arduino-Ftp-Server)
  Built by Khoi Hoang https://github.com/khoih-prog/FTP_Server_Teensy41
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#pragma once

#define TEENSY41_DEBUG_PORT                 Serial

// Debug Level from 0 to 4
#define _FTP_SERVER_LOGLEVEL_               2

#define USE_QN_ETHERNET         true
#define USE_NATIVE_ETHERNET     false
#define USE_ETHERNET_GENERIC    false

#define FTP_FILESYST            FTP_SDFAT2

// Default 2048
#define FTP_BUF_SIZE            8192

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "FTP_Server_Teensy41.hpp"
