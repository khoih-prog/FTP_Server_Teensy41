/****************************************************************************************************************************
  defines.h

  FTP_Server_Teensy41 is an FTP Server for Teensy 4.1 using SD, FS, etc. with QNEthernet or NativeEthernet

  Based on and modified from Arduino-Ftp-Server Library (https://github.com/gallegojm/Arduino-Ftp-Server)
  Built by Khoi Hoang https://github.com/khoih-prog/FTP_Server_Teensy41
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#define TEENSY41_DEBUG_PORT                 Serial

// Debug Level from 0 to 4
#define _FTP_SERVER_LOGLEVEL_               1

#if ( defined(CORE_TEENSY) && defined(__IMXRT1062__))
  #if (defined(ARDUINO_TEENSY41))
    // For Teensy 4.1
    #define BOARD_TYPE      "TEENSY 4.1"
    // Use true for QNEthernet or NativeEthernet Library, and false to use other Ethernet_Generic library
    #define USE_QN_ETHERNET         true
    #define USE_NATIVE_ETHERNET     false
  #else
    // For Teensy 4.0
    #define BOARD_TYPE      "TEENSY 4.0"
    // Use false for QNEthernet and NativeEthernet Library, and to use other Ethernet_Generic library
    #define USE_QN_ETHERNET         false
    #define USE_NATIVE_ETHERNET     false
  #endif
#else
  #error Only Teensy 4.1 supported
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif

// Use true  for ENC28J60 and UIPEthernet library (https://github.com/UIPEthernet/UIPEthernet)
// Use false for W5x00 and Ethernetx library      (https://www.arduino.cc/en/Reference/Ethernet)

#define USE_UIP_ETHERNET      false
#define USE_ETHERNET_GENERIC  false
#define USE_ETHERNET_ESP8266  false
#define USE_ETHERNET_ENC      false
#define USE_CUSTOM_ETHERNET   false

#if USE_QN_ETHERNET
  #include "QNEthernet.h"       // https://github.com/ssilverman/QNEthernet
  using namespace qindesign::network;
  //#warning Using QNEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "QNEthernet"

#elif USE_NATIVE_ETHERNET
  #include "NativeEthernet.h"
  //#warning Using NativeEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "NativeEthernet"

#elif USE_ETHERNET_GENERIC

  #include <SPI.h>

  ///////////////////////////////////////////////////////////

  // W5100 chips can have up to 4 sockets.  W5200 & W5500 can have up to 8 sockets.
  // Use EthernetLarge feature, Larger buffers, but reduced number of simultaneous connections/sockets (MAX_SOCK_NUM == 2)
  #define ETHERNET_LARGE_BUFFERS

  //////////////////////////////////////////////////////////

  #include "Ethernet_Generic.h"

  #if defined(ETHERNET_LARGE_BUFFERS)
    #define SHIELD_TYPE           "W5x00 using Ethernet_Generic Library with Large Buffer"
  #else
    #define SHIELD_TYPE           "W5x00 using Ethernet_Generic Library"
  #endif

#endif


#if (USE_NATIVE_ETHERNET || USE_ETHERNET_GENERIC)
// Enter a MAC address and IP address for your controller below.
#define NUMBER_OF_MAC      20

byte mac[][NUMBER_OF_MAC] =
{
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x02 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x03 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x04 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x06 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x07 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x08 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x09 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0A },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0B },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0C },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0D },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x0E },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x0F },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x10 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x12 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x13 },
  { 0xDE, 0xAD, 0xBE, 0xEF, 0xBE, 0x14 },
};

#else

#define USING_DHCP    false   //true

#if !USING_DHCP
  // Set the static IP address to use if the DHCP fails to assign
  IPAddress myIP(192, 168, 2, 241);
  IPAddress myNetmask(255, 255, 255, 0);
  IPAddress myGW(192, 168, 2, 1);
  //IPAddress mydnsServer(192, 168, 2, 1);
  IPAddress mydnsServer(8, 8, 8, 8);
#endif

#endif

#endif    //defines_h
