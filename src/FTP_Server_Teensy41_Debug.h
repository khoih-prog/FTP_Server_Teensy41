/****************************************************************************************************************************
  AsyncDNSServer_Teensy41_Debug.h
   
  AsyncDNSServer_Teensy41 is a Async UDP library for the Teensy41 using built-in Ethernet and QNEThernet
  
  Based on and modified from ESPAsyncUDP Library (https://github.com/me-no-dev/ESPAsyncUDP)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncDNSServer_Teensy41
  
  Version: 1.1.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.1.1   K Hoang      19/03/2022 Initial coding for Teensy 4.1 using built-in QNEthernet.
                                  Bump up version to v1.1.1 to sync with AsyncDNSServer_STM32 v1.1.1  
  *****************************************************************************************************************************/

#pragma once

#ifndef FTP_SERVER_TEENSY41_DEBUG_HPP
#define FTP_SERVER_TEENSY41_DEBUG_HPP

#ifdef TEENSY41_DEBUG_PORT
  #define DBG_PORT_FTP_SERVER      TEENSY41_DEBUG_PORT
#else
  #define DBG_PORT_FTP_SERVER      Serial
#endif

// Change _FTP_SERVER_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _FTP_SERVER_LOGLEVEL_
  #define _FTP_SERVER_LOGLEVEL_       0
#endif

/////////////////////////////////////////////////////////

const char FTP_SERVER_MARK[] = "[FTP] ";

#define FTP_SERVER_PRINT_MARK   FTP_SERVER_PRINT(FTP_SERVER_MARK)
#define FTP_SERVER_PRINT_SP     DBG_PORT_FTP_SERVER.print(" ")

#define FTP_SERVER_PRINT        DBG_PORT_FTP_SERVER.print
#define FTP_SERVER_PRINTLN      DBG_PORT_FTP_SERVER.println

/////////////////////////////////////////////////////////

#define FTP_LOGERROR(x)         if(_FTP_SERVER_LOGLEVEL_>0) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINTLN(x); }
#define FTP_LOGERROR0(x)        if(_FTP_SERVER_LOGLEVEL_>0) { FTP_SERVER_PRINT(x); }
#define FTP_LOGERROR1(x,y)      if(_FTP_SERVER_LOGLEVEL_>0) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(y); }
#define FTP_LOGERROR2(x,y,z)    if(_FTP_SERVER_LOGLEVEL_>0) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(z); }
#define FTP_LOGERROR3(x,y,z,w)  if(_FTP_SERVER_LOGLEVEL_>0) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(w); }
#define FTP_LOGERROR5(x,y,z,w,xx,yy)  if(_FTP_SERVER_LOGLEVEL_>0) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(w); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(xx); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(yy);}

/////////////////////////////////////////////////////////

#define FTP_LOGWARN(x)          if(_FTP_SERVER_LOGLEVEL_>1) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINTLN(x); }
#define FTP_LOGWARN0(x)         if(_FTP_SERVER_LOGLEVEL_>1) { FTP_SERVER_PRINT(x); }
#define FTP_LOGWARN1(x,y)       if(_FTP_SERVER_LOGLEVEL_>1) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(y); }
#define FTP_LOGWARN2(x,y,z)     if(_FTP_SERVER_LOGLEVEL_>1) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(z); }
#define FTP_LOGWARN3(x,y,z,w)   if(_FTP_SERVER_LOGLEVEL_>1) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(w); }
#define FTP_LOGWARN5(x,y,z,w,xx,yy)  if(_FTP_SERVER_LOGLEVEL_>1) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(w); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(xx); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(yy);}

/////////////////////////////////////////////////////////

#define FTP_LOGINFO(x)          if(_FTP_SERVER_LOGLEVEL_>2) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINTLN(x); }
#define FTP_LOGINFO0(x)         if(_FTP_SERVER_LOGLEVEL_>2) { FTP_SERVER_PRINT(x); }
#define FTP_LOGINFO1(x,y)       if(_FTP_SERVER_LOGLEVEL_>2) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(y); }
#define FTP_LOGINFO2(x,y,z)     if(_FTP_SERVER_LOGLEVEL_>2) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(z); }
#define FTP_LOGINFO3(x,y,z,w)   if(_FTP_SERVER_LOGLEVEL_>2) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(w); }
#define FTP_LOGINFO5(x,y,z,w,xx,yy)  if(_FTP_SERVER_LOGLEVEL_>2) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(w); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(xx); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(yy);}

/////////////////////////////////////////////////////////

#define FTP_LOGDEBUG(x)         if(_FTP_SERVER_LOGLEVEL_>3) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINTLN(x); }
#define FTP_LOGDEBUG0(x)        if(_FTP_SERVER_LOGLEVEL_>3) { FTP_SERVER_PRINT(x); }
#define FTP_LOGDEBUG1(x,y)      if(_FTP_SERVER_LOGLEVEL_>3) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(y); }
#define FTP_LOGDEBUG2(x,y,z)    if(_FTP_SERVER_LOGLEVEL_>3) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(z); }
#define FTP_LOGDEBUG3(x,y,z,w)  if(_FTP_SERVER_LOGLEVEL_>3) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(w); }
#define FTP_LOGDEBUG5(x,y,z,w,xx,yy)  if(_FTP_SERVER_LOGLEVEL_>3) { FTP_SERVER_PRINT_MARK; FTP_SERVER_PRINT(x); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(y); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(z); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(w); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINT(xx); FTP_SERVER_PRINT_SP; FTP_SERVER_PRINTLN(yy);}

/////////////////////////////////////////////////////////

#endif    //FTP_SERVER_TEENSY41_DEBUG_HPP
