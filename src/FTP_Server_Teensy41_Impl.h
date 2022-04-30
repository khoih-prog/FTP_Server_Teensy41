/**********************************************************************************************************************
  FTP_Server_Teensy41.hpp

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
   Copyright (c) 2014-2020 by Jean-Michel Gallego

   Please read file ReadMe.txt for instructions

   Use SdFat library 2.0.1 from Bill Greiman patched to add function
     file.getModifyDateTime( pdate, ptime )

   Use Ethernet library

   Commands implemented:
     USER, PASS, AUTH (AUTH only return 'not implemented' code)
     CDUP, CWD, PWD, QUIT, NOOP
     MODE, PASV, PORT, STRU, TYPE
     ABOR, DELE, LIST, NLST, MLST, MLSD
     APPE, RETR, STOR
     MKD,  RMD
     RNTO, RNFR
     MDTM, MFMT
     FEAT, SIZE
     SITE FREE

   Tested with those clients:
     under Windows:
       FTP Rush
       Filezilla
       WinSCP
       NcFTP, ncftpget, ncftpput
       Firefox
       command line ftp.exe
     under Ubuntu:
       gFTP
       Filezilla
       NcFTP, ncftpget, ncftpput
       lftp
       ftp
       Firefox
     under Android:
       AndFTP
       FTP Express
       Firefox
     with a second Arduino and sketch of SurferTim at
       http://playground.arduino.cc/Code/FTP

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

#ifndef FTP_SERVER_TEENSY41_IMPL_H
#define FTP_SERVER_TEENSY41_IMPL_H

#include "FTP_Server_Teensy41.hpp"

#include "FTP_Server_Teensy41_Debug.h"

#if USE_QN_ETHERNET
  #include "QNEthernet.h"             // https://github.com/ssilverman/QNEthernet
  
  #include "QNEthernetClient.h"       // https://github.com/ssilverman/QNEthernet
  #include "QNEthernetServer.h"       // https://github.com/ssilverman/QNEthernet
  #include "QNEthernetUDP.h"          // https://github.com/ssilverman/QNEthernet
  
  using namespace qindesign::network;
  #warning Using QNEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "using QNEthernet"
#elif USE_NATIVE_ETHERNET
  #include "NativeEthernet.h"
  #warning Using NativeEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
  #define SHIELD_TYPE           "using NativeEthernet"  
#else
  #include <Ethernet_Generic_Impl.h>
  
  #include <EthernetClient.h>             // https://github.com/khoih-prog/Ethernet_Generic
  #include <EthernetServer.h>             // https://github.com/khoih-prog/Ethernet_Generic
  #include <EthernetUdp.h>                // https://github.com/khoih-prog/Ethernet_Generic
#endif

////////////////////////////////////////////////////////////////////////////

ArduinoOutStream FtpDebug( FTP_SERIAL );

FtpServer::FtpServer( uint16_t _cmdPort, uint16_t _pasvPort )
  : ftpServer( _cmdPort ), dataServer( _pasvPort ),
    FtpOutCli( client ), FtpOutData( data )
{
  cmdPort = _cmdPort;
  pasvPort = _pasvPort;
}

void FtpServer::init( IPAddress _localIP )
{
  // Tells the ftp server to begin listening for incoming connection
  ftpServer.begin();

  localIp = _localIP == FTP_NULLIP() || (uint32_t) _localIP == 0 ? FTP_LOCALIP() : _localIP ;
  strcpy( user, FTP_USER );
  strcpy( pass, FTP_PASS );
  dataServer.begin();
  millisDelay = 0;
  cmdStage = FTP_Stop;
  iniVariables();
}

void FtpServer::credentials( const char * _user, const char * _pass )
{
  if ( strlen( _user ) > 0 && strlen( _user ) < FTP_CRED_SIZE )
    strcpy( user, _user );

  if ( strlen( _pass ) > 0 && strlen( _pass ) < FTP_CRED_SIZE )
    strcpy( pass, _pass );
}

void FtpServer::iniVariables()
{
  // Default for data port
  dataPort = FTP_DATA_PORT_DFLT;

  // Default Data connection is Active
  dataConn = FTP_NoConn;

  // Set the root directory
  strcpy( cwdName, "/" );

  rnfrCmd = false;
  transferStage = FTP_Close;
}

uint8_t FtpServer::service()
{
  if ((int32_t) ( millisDelay - millis() ) <= 0 )
  {
    if ( cmdStage == FTP_Stop )
    {
      if ( client.connected())
        disconnectClient();

      cmdStage = FTP_Init;
    }
    else if ( cmdStage == FTP_Init )      // Ftp server waiting for connection
    {
      abortTransfer();
      iniVariables();

      FTP_LOGWARN1(F(" Ftp server waiting for connection on port "), cmdPort);

      cmdStage = FTP_Client;
    }
    else if ( cmdStage == FTP_Client )    // Ftp server idle
    {
      if ( client && ! client.connected())
        client.stop();

      client = ftpServer.accept();

      if ( client.connected())            // A client connected
      {
        clientConnected();
        millisEndConnection = millis() + 1000L * FTP_AUTH_TIME_OUT; // wait client id for 10 s.
        cmdStage = FTP_User;
      }
    }
    else if ( readChar() > 0 )            // got response
    {
      processCommand();

      if ( cmdStage == FTP_Stop )
        millisEndConnection = millis() + 1000L * FTP_AUTH_TIME_OUT;  // wait authentication for 10 s.
      else if ( cmdStage < FTP_Cmd )
        millisDelay = millis() + 200;     // delay of 100 ms
      else
        millisEndConnection = millis() + 1000L * FTP_TIME_OUT;
    }
    else if ( ! client.connected() )
      cmdStage = FTP_Init;

    if ( transferStage == FTP_Retrieve )  // Retrieve data
    {
      if ( ! doRetrieve())
        transferStage = FTP_Close;
    }
    else if ( transferStage == FTP_Store ) // Store data
    {
      if ( ! doStore())
        transferStage = FTP_Close;
    }
    else if ( transferStage == FTP_List ||
              transferStage == FTP_Nlst)   // LIST or NLST
    {
      if ( ! doList())
        transferStage = FTP_Close;
    }
    else if ( transferStage == FTP_Mlsd ) // MLSD listing
    {
      if ( ! doMlsd())
        transferStage = FTP_Close;
    }
    else if ( cmdStage > FTP_Client &&
              ! ((int32_t) ( millisEndConnection - millis() ) > 0 ))
    {
      FtpOutCli << F("530 Timeout") << endl;
      millisDelay = millis() + 200;       // delay of 200 ms
      cmdStage = FTP_Stop;
    }    
  }

  return cmdStage | ( transferStage << 3 ) | ( dataConn << 6 );
}

void FtpServer::clientConnected()
{
  FTP_LOGWARN(F(" Client connected!"));
  
  FtpOutCli << F("220--- Welcome to FTP_Server_Teensy41 ---") << endl;
  FtpOutCli << F("220 -- ") << FTP_SERVER_TEENSY41_VERSION << F(" --") << endl;

  iCL = 0;
}

void FtpServer::disconnectClient()
{
	FTP_LOGWARN(F(" Disconnecting client"));

  abortTransfer();

  FtpOutCli << F("221 Goodbye") << endl;

  if ( client )
    client.stop();

  if ( data )
    data.stop();
}

bool FtpServer::processCommand()
{
  ///////////////////////////////////////
  //                                   //
  //      AUTHENTICATION COMMANDS      //
  //                                   //
  ///////////////////////////////////////

  //
  //  USER - User Identity
  //
  if ( CommandIs( "USER" ))
  {
    if ( ! strcmp( parameter, user ))
    {
      FtpOutCli << F("331 Ok. Password required") << endl;
      strcpy( cwdName, "/" );
      cmdStage = FTP_Pass;
    }
    else
    {
      FtpOutCli << F("530 ") << endl;
      cmdStage = FTP_Stop;
    }
  }
  //
  //  PASS - Password
  //
  else if ( CommandIs( "PASS" ))
  {
    if ( cmdStage != FTP_Pass )
    {
      FtpOutCli << F("503 ") << endl;
      cmdStage = FTP_Stop;
    }

    if ( ! strcmp( parameter, pass ))
    {
      FTP_LOGWARN(F(" Authentication Ok. Waiting for commands."));

      FtpOutCli << F("230 Ok") << endl;
      cmdStage = FTP_Cmd;
    }
    else
    {
      FtpOutCli << F("530 ") << endl;
      cmdStage = FTP_Stop;
    }
  }
  //
  //  FEAT - New Features
  //
  else if ( CommandIs( "FEAT" ))
  {
    FtpOutCli << F("211-Extensions supported:") << endl;
    FtpOutCli << F(" MLST type*;modify*;size*;") << endl;
    FtpOutCli << F(" MLSD") << endl;
    FtpOutCli << F(" MDTM") << endl;
    FtpOutCli << F(" MFMT") << endl;
    FtpOutCli << F(" SIZE") << endl;
    FtpOutCli << F(" SITE FREE") << endl;
    FtpOutCli << F("211 End.") << endl;
  }
  //
  //  AUTH - Not implemented
  //
  else if ( CommandIs( "AUTH" ))
    FtpOutCli << F("502 ") << endl;
  //
  //  Unrecognized commands at stage of authentication
  //
  else if ( cmdStage < FTP_Cmd )
  {
    FtpOutCli << F("530 ") << endl;
    
    cmdStage = FTP_Stop;
  }

  ///////////////////////////////////////
  //                                   //
  //      ACCESS CONTROL COMMANDS      //
  //                                   //
  ///////////////////////////////////////

  //
  //  PWD - Print Directory
  //
  else if ( CommandIs( "PWD" ) || ( CommandIs( "CWD" ) && ParameterIs( "." )))
  {
    FtpOutCli << F("257 \"") << cwdName << F("\"") << F(" is your current directory") << endl;
  }
  //
  //  CDUP - Change to Parent Directory
  //
  else if ( CommandIs( "CDUP" ) || ( CommandIs( "CWD" ) && ParameterIs( ".." )))
  {
    bool ok = false;

    if ( strlen( cwdName ) > 1 )           // do nothing if cwdName is root
    {
      // if cwdName ends with '/', remove it (must not append)
      if ( cwdName[ strlen( cwdName ) - 1 ] == '/' )
        cwdName[ strlen( cwdName ) - 1 ] = 0;

      // search last '/'
      char * pSep = strrchr( cwdName, '/' );
      ok = pSep > cwdName;

      // if found, ends the string on its position
      if ( ok )
      {
        * pSep = 0;
        ok = exists( cwdName );
      }
    }

    // if an error appends, move to root
    if ( ! ok )
      strcpy( cwdName, "/" );

    FtpOutCli << F("250 Ok. Current directory is ") << cwdName << endl;
  }
  //
  //  CWD - Change Working Directory
  //
  else if ( CommandIs( "CWD" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makeExistsPath( path ))
    {
      strcpy( cwdName, path );
      
      FtpOutCli << F("250 Directory changed to ") << cwdName << endl;
    }
  }
  //
  //  QUIT
  //
  else if ( CommandIs( "QUIT" ))
  {
    FtpOutCli << F("221 Goodbye") << endl;
    
    disconnectClient();
    cmdStage = FTP_Stop;
  }

  ///////////////////////////////////////
  //                                   //
  //    TRANSFER PARAMETER COMMANDS    //
  //                                   //
  ///////////////////////////////////////

  //
  //  MODE - Transfer Mode
  //
  else if ( CommandIs( "MODE" ))
  {
    if ( ParameterIs( "S" ))
      FtpOutCli << F("200 S Ok") << endl;
    else
      FtpOutCli << F("504 Only S(tream) is supported") << endl;
  }
  //
  //  PASV - Passive Connection management
  //
  else if ( CommandIs( "PASV" ))
  {
    data.stop();
    dataServer.begin();

    if ((((uint32_t) FTP_LOCALIP()) & ((uint32_t) Ethernet.subnetMask())) ==
        (((uint32_t) client.remoteIP()) & ((uint32_t) Ethernet.subnetMask())))
      dataIp = FTP_LOCALIP();
    else
      dataIp = localIp;

    dataPort = pasvPort;

    FTP_LOGWARN(F(" Connection management set to passive"));
    FTP_LOGWARN3(F(" Listening at "), dataIp, F(":"), dataPort);
    
#if 1
    FtpOutCli << F("227 Entering Passive Mode") << F(" (")
              << dataIp << F(", port ") << dataPort << F(")") << endl;
#else
    FtpOutCli << F("227 Entering Passive Mode") << F(" (")
              << int( dataIp[0]) << F(",") << int( dataIp[1]) << F(",")
              << int( dataIp[2]) << F(",") << int( dataIp[3]) << F(",")
              << ( dataPort >> 8 ) << F(",") << ( dataPort & 255 ) << F(")") << endl;
#endif
       
    dataConn = FTP_Pasive;
  }
  //
  //  PORT - Data Port
  //
  else if ( CommandIs( "PORT" ))
  {
    data.stop();
    // get IP of data client
    dataIp[ 0 ] = atoi( parameter );
    char * p = strchr( parameter, ',' );

    for ( uint8_t i = 1; i < 4; i ++ )
    {
      dataIp[ i ] = atoi( ++ p );
      p = strchr( p, ',' );
    }

    // get port of data client
    dataPort = 256 * atoi( ++ p );
    p = strchr( p, ',' );
    dataPort += atoi( ++ p );

    if ( p == NULL )
      FtpOutCli << F("501 Can't interpret parameters") << endl;
    else
    {
      FTP_LOGWARN3(F(" Data IP set to "), dataIp, F(", Data port set to "), dataPort);

      FtpOutCli << F("200 PORT command successful") << endl;
      dataConn = FTP_Active;
    }
  }
  //
  //  STRU - File Structure
  //
  else if ( CommandIs( "STRU" ))
  {
    if ( ParameterIs( "F" ))
      FtpOutCli << F("200 F Ok") << endl;
    // else if( ParameterIs( "R" ))
    //  FtpOutCli << F("200 B Ok") << endl;
    else
      FtpOutCli << F("504 Only F(ile) is supported") << endl;
  }
  //
  //  TYPE - Data Type
  //
  else if ( CommandIs( "TYPE" ))
  {
    if ( ParameterIs( "A" ))
      FtpOutCli << F("200 TYPE is now ASCII") << endl;
    else if ( ParameterIs( "I" ))
      FtpOutCli << F("200 TYPE is now 8-bit binary") << endl;
    else
      FtpOutCli << F("504 Unknown TYPE") << endl;
  }

  ///////////////////////////////////////
  //                                   //
  //        FTP SERVICE COMMANDS       //
  //                                   //
  ///////////////////////////////////////

  //
  //  ABOR - Abort
  //
  else if ( CommandIs( "ABOR" ))
  {
    abortTransfer();
    FtpOutCli << F("226 Data connection closed") << endl;
  }
  //
  //  DELE - Delete a File
  //
  else if ( CommandIs( "DELE" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makeExistsPath( path ))
    {
      if ( remove( path ))
        FtpOutCli << F("250 Deleted ") << parameter << endl;
      else
        FtpOutCli << F("450 Can't delete ") << parameter << endl;
    }
  }
  //
  //  LIST - List
  //  NLST - Name List
  //  MLSD - Listing for Machine Processing (see RFC 3659)
  //
  else if ( CommandIs( "LIST" ) || CommandIs( "NLST" ) || CommandIs( "MLSD" ))
  {
    if ( dataConnect())
    {
      if ( openDir( & dir ))
      {
        nbMatch = 0;

        if ( CommandIs( "LIST" ))
          transferStage = FTP_List;
        else if ( CommandIs( "NLST" ))
          transferStage = FTP_Nlst;
        else
          transferStage = FTP_Mlsd;
      }
      else
        data.stop();
    }
  }
  //
  //  MLST - Listing for Machine Processing (see RFC 3659)
  //
  else if ( CommandIs( "MLST" ))
  {
    char path[ FTP_CWD_SIZE ];
    uint16_t dat, tim;
    char dtStr[ 15 ];
    bool isdir;

    if ( haveParameter() && makeExistsPath( path ))
    {
      if ( ! getFileModTime( path, & dat, & tim ))
        FtpOutCli << F("550 Unable to retrieve time for ") << parameter << endl;
      else
      {
        isdir = isDir( path );
        FtpOutCli << F("250-Begin") << endl
                  << F(" Type=") << ( isdir ? F("dir") : F("file"))
                  << F(";Modify=") << makeDateTimeStr( dtStr, dat, tim );

        if ( ! isdir )
        {
          if ( file.open( path, O_READ ))
          {
            FtpOutCli << F(";Size=") << long( file.fileSize());
            file.close();
          }
        }

        FtpOutCli << F("; ") << path << endl
                  << F("250 End.") << endl;
      }
    }
  }
  //
  //  NOOP
  //
  else if ( CommandIs( "NOOP" ))
    FtpOutCli << F("200 Zzz...") << endl;
  //
  //  RETR - Retrieve
  //
  else if ( CommandIs( "RETR" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makeExistsPath( path ))
    {
      if ( ! file.open( path, O_READ ))
        FtpOutCli << F("450 Can't open ") << parameter << endl;
      else if ( dataConnect( false ))
      {
        FTP_LOGWARN1(F(" Sending "), parameter);

        FtpOutCli << F("150-Connected to port ") << dataPort << endl;
        FtpOutCli << F("150 ") << long( file.fileSize()) << F(" bytes to download") << endl;
        
        millisBeginTrans = millis();
        bytesTransfered = 0;
        transferStage = FTP_Retrieve;
      }
    }
  }
  //
  //  STOR - Store
  //  APPE - Append
  //
  else if ( CommandIs( "STOR" ) || CommandIs( "APPE" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makePath( path ))
    {
      bool open;

      if ( exists( path ))
        open = file.open( path, O_WRITE | ( CommandIs( "APPE" ) ? O_APPEND : O_CREAT ));
      else
        open = file.open( path, O_WRITE | O_CREAT );

      if ( ! open )
        FtpOutCli << F("451 Can't open/create ") << parameter << endl;
      else if ( ! dataConnect())
        file.close();
      else
      {
        FTP_LOGWARN1(F(" Receiving "), parameter);

        millisBeginTrans = millis();
        bytesTransfered = 0;
        transferStage = FTP_Store;
      }
    }
  }
  //
  //  MKD - Make Directory
  //
  else if ( CommandIs( "MKD" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makePath( path ))
    {
      if ( exists( path ))
        FtpOutCli << F("521 \"") << parameter << F("\" directory already exists") << endl;
      else
      {
        FTP_LOGWARN1(F(" Creating directory "), parameter);

        if ( makeDir( path ))
          FtpOutCli << F("257 \"") << parameter << F("\"") << F(" created") << endl;
        else
          FtpOutCli << F("550 Can't create \"") << parameter << F("\"") << endl;
      }
    }
  }
  //
  //  RMD - Remove a Directory
  //
  else if ( CommandIs( "RMD" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makeExistsPath( path ))
    {
      if ( removeDir( path ))
      {
        FTP_LOGWARN1(F(" Deleting "), path);

        FtpOutCli << F("250 \"") << parameter << F("\" deleted") << endl;
      }
      else
        FtpOutCli << F("550 Can't remove \"") << parameter << F("\". Directory not empty?") << endl;
    }
  }
  //
  //  RNFR - Rename From
  //
  else if ( CommandIs( "RNFR" ))
  {
    rnfrName[ 0 ] = 0;

    if ( haveParameter() && makeExistsPath( rnfrName ))
    {
      FTP_LOGWARN1(F(" Ready for renaming "), rnfrName);

      FtpOutCli << F("350 RNFR accepted - file exists, ready for destination") << endl;
      rnfrCmd = true;
    }
  }
  //
  //  RNTO - Rename To
  //
  else if ( CommandIs( "RNTO" ))
  {
    char path[ FTP_CWD_SIZE ];
    char dirp[ FTP_FIL_SIZE ];

    if ( strlen( rnfrName ) == 0 || ! rnfrCmd )
      FtpOutCli << F("503 Need RNFR before RNTO") << endl;
    else if ( haveParameter() && makePath( path ))
    {
      if ( exists( path ))
        FtpOutCli << F("553 ") << parameter << F(" already exists") << endl;
      else
      {
        strcpy( dirp, path );
        char * psep = strrchr( dirp, '/' );
        bool fail = psep == NULL;

        if ( ! fail )
        {
          if ( psep == dirp )
            psep ++;

          * psep = 0;
          fail = ! isDir( dirp );

          if ( fail )
            FtpOutCli << F("550 \"") << dirp << F("\" is not directory") << endl;
          else
          {
            FTP_LOGWARN3(F(" Renaming "), rnfrName, F(" to "), path);

            if ( rename( rnfrName, path ))
              FtpOutCli << F("250 File successfully renamed or moved") << endl;
            else
              fail = true;
          }
        }

        if ( fail )
          FtpOutCli << F("451 Rename/move failure") << endl;
      }
    }

    rnfrCmd = false;
  }
  /*
    //
    //  SYST - System
    //
    else if( CommandIs( "SYST" ))
    FtpOutCli << F("215 MSDOS") << endl;
  */

  ///////////////////////////////////////
  //                                   //
  //   EXTENSIONS COMMANDS (RFC 3659)  //
  //                                   //
  ///////////////////////////////////////

  //
  //  MDTM && MFMT - File Modification Time (see RFC 3659)
  //
  else if ( CommandIs( "MDTM" ) || CommandIs( "MFMT" ))
  {
    if ( haveParameter())
    {
      char path[ FTP_CWD_SIZE ];
      char * fname = parameter;
      uint16_t year;
      uint8_t month, day, hour, minute, second, setTime;
      char dt[ 15 ];
      bool mdtm = CommandIs( "MDTM" );

      setTime = getDateTime( dt, & year, & month, & day, & hour, & minute, & second );
      // fname point to file name
      fname += setTime;

      if ( strlen( fname ) <= 0 )
        FtpOutCli << "501 No file name" << endl;
      else if ( makeExistsPath( path, fname ))
      {
        if ( setTime ) // set file modification time
        {
          if ( timeStamp( path, year, month, day, hour, minute, second ))
            FtpOutCli << "213 " << dt << endl;
          else
            FtpOutCli << "550 Unable to modify time" << endl;
        }
        else if ( mdtm ) // get file modification time
        {
          uint16_t dat, tim;
          char dtStr[ 15 ];

          if ( getFileModTime( path, & dat, & tim ))
            FtpOutCli << "213 " << makeDateTimeStr( dtStr, dat, tim ) << endl;
          else
            FtpOutCli << "550 Unable to retrieve time" << endl;
        }
      }
    }
  }
  //
  //  SIZE - Size of the file
  //
  else if ( CommandIs( "SIZE" ))
  {
    char path[ FTP_CWD_SIZE ];

    if ( haveParameter() && makeExistsPath( path ))
    {
      if ( ! file.open( path ))
        FtpOutCli << F("450 Can't open ") << parameter << endl;
      else
      {
        FtpOutCli << F("213 ") << long( file.fileSize()) << endl;
        file.close();
      }
    }
  }
  //
  //  SITE - System command
  //
  else if ( CommandIs( "SITE" ))
  {
    if ( ParameterIs( "FREE" ))
    {
      uint32_t capa = capacity();

      if (( capa >> 10 ) < 1000 ) // less than 1 Giga
        FtpOutCli << F("200 ") << free() << F(" kB free of ")
                  << capa << F(" kB capacity") << endl;
      else
        FtpOutCli << F("200 ") << ( free() >> 10 ) << F(" MB free of ")
                  << ( capa >> 10 ) << F(" MB capacity") << endl;
    }
    else
      FtpOutCli << F("500 Unknown SITE command ") << parameter << endl;
  }
  //
  //  Unrecognized commands ...
  //
  else
    FtpOutCli << F("500 Unknown command") << endl;

  return true;
}

int FtpServer::dataConnect( bool out150 )
{
  if ( ! data.connected())
  {
    if ( dataConn == FTP_Pasive )
    {
      uint16_t count = 1000; // wait up to a second

      while ( ! data.connected() && count -- > 0 )
      {
        data = dataServer.accept();

        delay( 1 );
      }
    }
    else if ( dataConn == FTP_Active )
      data.connect( dataIp, dataPort );
  }

  if ( ! data.connected())
    FtpOutCli << F("425 No data connection") << endl;
  else if ( out150 )
    FtpOutCli << F("150 Accepted data connection to port ") << dataPort << endl;

  return data.connected();
}

bool FtpServer::dataConnected()
{
  if ( data.connected())
    return true;

  data.stop();
  FtpOutCli << F("426 Data connection closed. Transfer aborted") << endl;
  transferStage = FTP_Close;

  return false;
}

bool FtpServer::openDir( FTP_DIR * pdir )
{
  bool openD;

  if ( cwdName == 0 )
    openD = pdir->open( "/" );
  else
    openD = pdir->open( cwdName );

  if ( ! openD )
    FtpOutCli << F("550 Can't open directory ") << cwdName << endl;

  return openD;
}

bool FtpServer::doRetrieve()
{
  if ( ! dataConnected())
  {
    file.close();

    return false;
  }

  int16_t nb = file.read( buf, FTP_BUF_SIZE );

  if ( nb > 0 )
  {
    data.write( buf, nb );
    bytesTransfered += nb;
    return true;
  }

  closeTransfer();

  return false;
}

bool FtpServer::doStore()
{
  int16_t na = data.available();

  if ( na == 0 )
  {
    if ( data.connected())
      return true;
    else
    {
      closeTransfer();
      return false;
    }
  }

  if ( na > FTP_BUF_SIZE )
    na = FTP_BUF_SIZE;

  int16_t nb = data.read((uint8_t *) buf, na );
  int16_t rc = 0;

  if ( nb > 0 )
  {
    // FtpDebug << millis() << " " << nb << endl;
    rc = file.write( buf, nb );
    bytesTransfered += nb;
  }

  if ( nb < 0 || rc == nb  )
    return true;

  FtpOutCli << F("552 Probably insufficient storage space") << endl;
  file.close();
  data.stop();

  return false;
}

bool FtpServer::doList()
{
  if ( ! dataConnected())
  {
    dir.close();
    return false;
  }

#if FTP_FILESYST == FTP_FATFS

  if ( dir.nextFile())
  {
    if ( dir.isDir())
      FtpOutData << F("+/,\t");
    else
      FtpOutData << F("+r,s") << long( dir.fileSize()) << F(",\t");

    FtpOutData << dir.fileName() << endl;
    nbMatch ++;

    return true;
  }
  
#else

  if ( file.openNext( &dir, O_RDONLY ))
  {
    if ( file.isDir())
      FtpOutData << F("+/,\t");
    else
      FtpOutData << F("+r,s") << long( file.fileSize()) << F(",\t");

    file.printName( & data );
    FtpOutData << endl;
    file.close();
    nbMatch ++;

    return true;
  }

#endif

  FtpOutCli << F("226 ") << nbMatch << F(" matches total") << endl;
  dir.close();
  data.stop();

  return false;
}

bool FtpServer::doMlsd()
{
  if ( ! dataConnected())
  {
    dir.close();
    return false;
  }

#if FTP_FILESYST == FTP_FATFS

  if ( dir.nextFile())
  {
    char dtStr[ 15 ];
    FtpOutData << F("Type=") << ( dir.isDir() ? F("dir") : F("file"))
               << F(";Modify=") << makeDateTimeStr( dtStr, dir.fileModDate(), dir.fileModTime())
               << F(";Size=") << long( dir.fileSize())
               << F("; ") << dir.fileName() << endl;
    nbMatch ++;

    return true;
  }

#else

  if ( file.openNext( &dir, O_RDONLY ))
  {
    char dtStr[ 15 ];
    uint16_t filelwd, filelwt;
    bool gfmt = getFileModTime( & filelwd, & filelwt );

    if ( gfmt )
    {
      FtpOutData << F("Type=") << ( file.isDir() ? F("dir") : F("file"))
                 << F(";Modify=") << makeDateTimeStr( dtStr, filelwd, filelwt )
                 << F(";Size=") << long( file.fileSize()) << F("; ");
      file.printName( & data );
      FtpOutData << endl;
      nbMatch ++;
    }

    file.close();

    return gfmt;
  }

#endif

  FtpOutCli << F("226-options: -a -l") << endl;
  FtpOutCli << F("226 ") << nbMatch << F(" matches total") << endl;
  dir.close();
  data.stop();

  return false;
}

void FtpServer::closeTransfer()
{
  uint32_t deltaT = (int32_t) ( millis() - millisBeginTrans );

  if ( deltaT > 0 && bytesTransfered > 0 )
  {
    FTP_LOGWARN3(F(" Transfer completed in (ms) "), deltaT, F(", Speed (kbytes/s) "), bytesTransfered / deltaT);

    FtpOutCli << F("226-File successfully transferred") << endl;
    FtpOutCli << F("226 ") << deltaT << F(" ms, ")
              << bytesTransfered / deltaT << F(" kbytes/s") << endl;
  }
  else
    FtpOutCli << F("226 File successfully transferred") << endl;

  file.close();
  data.stop();
}

void FtpServer::abortTransfer()
{
  if ( transferStage != FTP_Close )
  {
    file.close();
    dir.close();
    FtpOutCli << F("426 Transfer aborted") << endl;

    FTP_LOGWARN(F(" Transfer aborted!"));

    transferStage = FTP_Close;
  }

  //  if( data.connected())
  data.stop();
}

// Read a char from client connected to ftp server
//
//  update cmdLine and command buffers, iCL and parameter pointers
//
//  return:
//    -2 if buffer cmdLine is full
//    -1 if line not completed
//     0 if empty line received
//    length of cmdLine (positive) if no empty line received

int8_t FtpServer::readChar()
{
  int8_t rc = -1;

  if ( client.available())
  {
    char c = client.read();

    FTP_LOGDEBUG0(c);

    if ( c == '\\' )
      c = '/';

    if ( c != '\r' )
    {
      if ( c != '\n' )
      {
        if ( iCL < FTP_CMD_SIZE )
          cmdLine[ iCL ++ ] = c;
        else
          rc = -2; //  Line too long
      }
      else
      {
        cmdLine[ iCL ] = 0;
        command[ 0 ] = 0;
        parameter = NULL;

        // empty line?
        if ( iCL == 0 )
          rc = 0;
        else
        {
          rc = iCL;
          // search for space between command and parameter
          parameter = strchr( cmdLine, ' ' );

          if ( parameter != NULL )
          {
            if ( parameter - cmdLine > 4 )
              rc = -2; // Syntax error
            else
            {
              strncpy( command, cmdLine, parameter - cmdLine );
              command[ parameter - cmdLine ] = 0;

              while ( * ( ++ parameter ) == ' ' );
            }
          }
          else if ( strlen( cmdLine ) > 4 )
            rc = -2; // Syntax error.
          else
            strcpy( command, cmdLine );

          iCL = 0;
        }
      }
    }

    if ( rc > 0 )
    {
      for ( uint8_t i = 0 ; i < strlen( command ); i ++ )
        command[ i ] = toupper( command[ i ] );
    }

    if ( rc == -2 )
    {
      iCL = 0;
      FtpOutCli << F("500 Syntax error") << endl;
    }
  }

  return rc;
}

bool FtpServer::haveParameter()
{
  if ( parameter != NULL && strlen( parameter ) > 0 )
    return true;

  FtpOutCli << "501 No file name" << endl;

  return false;
}

// Make complete path/name from cwdName and param
//
// 3 possible cases: param can be absolute path, relative path or only the name
//
// parameter:
//   fullName : where to store the path/name
//
// return:
//    true, if done

bool FtpServer::makePath( char * fullName, char * param )
{
  if ( param == NULL )
    param = parameter;

  // Root or empty?
  if ( strcmp( param, "/" ) == 0 || strlen( param ) == 0 )
  {
    strcpy( fullName, "/" );
    return true;
  }

  // If relative path, concatenate with current dir
  if ( param[0] != '/' )
  {
    strcpy( fullName, cwdName );

    if ( fullName[ strlen( fullName ) - 1 ] != '/' )
      strncat( fullName, "/", FTP_CWD_SIZE );

    strncat( fullName, param, FTP_CWD_SIZE );
  }
  else
    strcpy( fullName, param );

  // If ends with '/', remove it
  uint16_t strl = strlen( fullName ) - 1;

  if ( fullName[ strl ] == '/' && strl > 1 )
    fullName[ strl ] = 0;

  if ( strlen( fullName ) >= FTP_CWD_SIZE )
  {
    FtpOutCli << F("500 Command line too long") << endl;
    return false;
  }

  for ( uint8_t i = 0; i < strlen( fullName ); i ++ )
  {
    if ( ! legalChar( fullName[i]))
    {
      FtpOutCli << F("553 File name not allowed") << endl;
      return false;
    }
  }

  return true;
}

bool FtpServer::makeExistsPath( char * path, char * param )
{
  if ( ! makePath( path, param ))
    return false;

  if ( exists( path ))
    return true;

  FtpOutCli << F("550 ") << path << F(" not found.") << endl;

  return false;
}

// Calculate year, month, day, hour, minute and second
//   from first parameter sent by MDTM command (YYYYMMDDHHMMSS)
// Accept longer parameter YYYYMMDDHHMMSSmmm where mmm are milliseconds
//   but don't take in account additional digits
//
// parameters:
//   dt: 15 length string for 14 digits and terminator
//   pyear, pmonth, pday, phour, pminute and psecond: pointer of
//     variables where to store data
//
// return:
//    0 if parameter is not YYYYMMDDHHMMSS
//    length of parameter + space
//
// Date/time are expressed as a 14 digits long string
//   terminated by a space and followed by name of file

uint8_t FtpServer::getDateTime( char * dt, uint16_t * pyear, uint8_t * pmonth, uint8_t * pday,
                                uint8_t * phour, uint8_t * pminute, uint8_t * psecond )
{
  uint8_t i;
  dt[ 0 ] = 0;

  if ( strlen( parameter ) < 15 ) //|| parameter[ 14 ] != ' ' )
    return 0;

  for ( i = 0; i < 14; i ++ )
  {
    if ( ! isdigit( parameter[ i ]))
      return 0;
  }

  for ( i = 14; i < 18; i ++ )
  {
    if ( parameter[ i ] == ' ' )
      break;
    else if ( ! isdigit( parameter[ i ]))
      return 0;
  }

  if ( i == 18 )
    return 0;

  i ++ ;

  strncpy( dt, parameter, 14 );
  dt[ 14 ] = 0;
  * psecond = atoi( dt + 12 );
  dt[ 12 ] = 0;
  * pminute = atoi( dt + 10 );
  dt[ 10 ] = 0;
  * phour = atoi( dt + 8 );
  dt[ 8 ] = 0;
  * pday = atoi( dt + 6 );
  dt[ 6 ] = 0 ;
  * pmonth = atoi( dt + 4 );
  dt[ 4 ] = 0 ;
  * pyear = atoi( dt );

  strncpy( dt, parameter, 14 );

	FTP_LOGWARN1(F(" File: "), (char *) ( parameter + i ));
  FTP_LOGWARN5(F(" Modification time yy/mm/dd: "), int (* pyear), F("/"), int (* pmonth), F("/"), int (* pday));
  FTP_LOGWARN5(F(" Modification time hh:mm:ss: "), int (* phour), F(":"), int (* pminute), F(":"), int (* psecond));
  
  return i;
}

// Create string YYYYMMDDHHMMSS from date and time
//
// parameters:
//    date, time
//    tstr: where to store the string. Must be at least 15 characters long
//
// return:
//    pointer to tstr

char * FtpServer::makeDateTimeStr( char * tstr, uint16_t date, uint16_t time )
{
  sprintf( tstr, "%04u%02u%02u%02u%02u%02u",
           (( date & 0xFE00 ) >> 9 ) + 1980, ( date & 0x01E0 ) >> 5, date & 0x001F,
           ( time & 0xF800 ) >> 11, ( time & 0x07E0 ) >> 5, ( time & 0x001F ) << 1 );

  return tstr;
}

// Return true if path points to a directory

bool FtpServer::isDir( char * path )
{
#if FTP_FILESYST == FTP_FATFS
  return FTP_FS.isDir( path );
#else
  FTP_FILE file;
  bool res;

  if ( ! file.open( path, O_READ ))
    return false;

  res = file.isDir();
  file.close();

  return res;
#endif
}

bool FtpServer::timeStamp( char * path, uint16_t year, uint8_t month, uint8_t day,
                           uint8_t hour, uint8_t minute, uint8_t second )
{
#if FTP_FILESYST == FTP_FATFS
  return FTP_FS.timeStamp( path, year, month, day, hour, minute, second );
#else
  FTP_FILE file;
  bool res;

  if ( ! file.open( path, O_RDWR ))
    return false;

  res = file.timestamp( T_WRITE, year, month, day, hour, minute, second );
  file.close();

  return res;
#endif
}

bool FtpServer::getFileModTime( char * path, uint16_t * pdate, uint16_t * ptime )
{
#if FTP_FILESYST == FTP_FATFS
  return FTP_FS.getFileModTime( path, pdate, ptime );
#else
  //  FTP_FILE file;
  bool res;

  if ( ! file.open( path, O_READ ))
    return false;

  res = getFileModTime( pdate, ptime );
  file.close();

  return res;
#endif
}

// Assume SD library is SdFat (or family) and file is open

#if FTP_FILESYST != FTP_FATFS
bool FtpServer::getFileModTime( uint16_t * pdate, uint16_t * ptime )
{
#if FTP_FILESYST == FTP_SDFAT1 || FTP_FILESYST == FTP_SPIFM
  dir_t d;

  if ( ! file.dirEntry( & d ))
    return false;

  * pdate = d.lastWriteDate;
  * ptime = d.lastWriteTime;

  return true;
#elif FTP_FILESYST == FTP_SDFAT2
  return file.getModifyDateTime( pdate, ptime );
#endif
}
#endif

#endif    // FTP_SERVER_TEENSY41_IMPL_H
