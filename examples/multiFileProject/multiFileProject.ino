/****************************************************************************************************************************
  multiFileProject.ino
  FTP_Server_Teensy41 is an FTP Server for Teensy 4.1 using SD, FS, etc. with QNEthernet or NativeEthernet
  
  Based on and modified from Arduino-Ftp-Server Library (https://github.com/gallegojm/Arduino-Ftp-Server)
  Built by Khoi Hoang https://github.com/khoih-prog/FTP_Server_Teensy41
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if ( defined(CORE_TEENSY) && defined(__IMXRT1062__)) 
  #if (defined(ARDUINO_TEENSY41))
    // For Teensy 4.1
    #define BOARD_TYPE      "TEENSY 4.1"
  #else
    // For Teensy 4.0
    #define BOARD_TYPE      "TEENSY 4.0"
  #endif
#else
  #error Only Teensy 4.1 supported
#endif

#define FTP_SERVER_TEENSY41_VERSION_MIN_TARGET      "FTP_Server_Teensy41 v1.2.1"
#define FTP_SERVER_TEENSY41_VERSION_MIN             1002001

#define FTP_FILESYST    FTP_SDFAT2

// Default 2048
#define FTP_BUF_SIZE    8192

#include "multiFileProject.h"

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "FTP_Server_Teensy41.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(FTP_SERVER_TEENSY41_VERSION);

#if defined(FTP_SERVER_TEENSY41_VERSION_MIN)
  if (FTP_SERVER_TEENSY41_VERSION_INT < FTP_SERVER_TEENSY41_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(FTP_SERVER_TEENSY41_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
