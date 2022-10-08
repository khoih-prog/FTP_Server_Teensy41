/**********************************************************************************************************************
  FTP_Server_SDFAT2.ino
   
  FTP_Server_Teensy41 is an FTP Server for Teensy 4.1 using SD, FS, etc. with QNEthernet or NativeEthernet
  
  Based on and modified from Arduino-Ftp-Server Library (https://github.com/gallegojm/Arduino-Ftp-Server)
  Built by Khoi Hoang https://github.com/khoih-prog/FTP_Server_Teensy41
 ***********************************************************************************************************************/

#include "defines.h"

#include <SD.h>
#include <SPI.h>

#define FTP_FILESYST                  FTP_SDFAT2

// Default 2048
#define FTP_BUF_SIZE                  4096

#define FTP_USER_NAME_LEN             64        // Max permissible and default are 64
#define FTP_USER_PWD_LEN             128        // Max permissible and default are 128

#include <FTP_Server_Teensy41.h>

// Object for FtpServer
//  Command port and Data port in passive mode can be defined here
// FtpServer ftpSrv( 221, 25000 );
// FtpServer ftpSrv( 421 ); // Default data port in passive mode is 55600
FtpServer ftpSrv; // Default command port is 21 ( !! without parenthesis !! )

File myFile;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = BUILTIN_SDCARD;    //10;

SDClass myfs;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

/*******************************************************************************
**                                                                            **
**                               INITIALISATION                               **
**                                                                            **
*******************************************************************************/

#define FTP_ACCOUNT       "teensy4x"
#define FTP_PASSWORD      "ftp_test"

void initNetwork()
{
#if USE_QN_ETHERNET
  Serial.println(F("=========== USE_QN_ETHERNET ==========="));
#elif USE_NATIVE_ETHERNET
  Serial.println(F("========= USE_NATIVE_ETHERNET ========="));
#elif USE_ETHERNET_GENERIC
  Serial.println(F("========= USE_ETHERNET_GENERIC ========"));
#elif USE_WIFI_NINA
  Serial.println(F("============ USE_WIFI_NINA ============"));
#else
  Serial.println(F("======================================="));
#endif

#if USE_WIFI_NINA
  // check for the WiFi module:
  // Using this setPins() with Adafruit WiFiNINA library
  //WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
 
  // Start the Ethernet connection, using DHCP
  Serial.print("Initialize WiFi using DHCP => ");
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: ");  Serial.println(WiFi.localIP());

#elif USE_NATIVE_ETHERNET

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], ip);
  Ethernet.begin(mac[index]);

  Serial.print(F("Using mac index = "));
  Serial.println(index);

  Serial.print(F("Connected! IP address: "));
  Serial.println(Ethernet.localIP());

#elif USE_QN_ETHERNET

#if USING_DHCP
  // Start the Ethernet connection, using DHCP
  Serial.print("Initialize Ethernet using DHCP => ");
  Ethernet.begin();
  // give the Ethernet shield minimum 1 sec for DHCP and 2 secs for staticP to initialize:
  delay(1000);
#else
  // Start the Ethernet connection, using static IP
  Serial.print("Initialize Ethernet using static IP => ");
  Ethernet.begin(myIP, myNetmask, myGW);
  Ethernet.setDNSServerIP(mydnsServer);
#endif

  if (!Ethernet.waitForLocalIP(5000))
  {
    Serial.println("Failed to configure Ethernet");

    if (!Ethernet.linkStatus())
    {
      Serial.println("Ethernet cable is not connected.");
    }

    // Stay here forever
    while (true)
    {
      delay(1);
    }
  }
  else
  {
    Serial.print("IP Address = ");
    Serial.println(Ethernet.localIP());
  }

  // give the Ethernet shield minimum 1 sec for DHCP and 2 secs for staticP to initialize:
  //delay(2000);

#else

  FTP_LOGWARN(F("Default SPI pinout:"));
  FTP_LOGWARN1(F("MOSI:"), MOSI);
  FTP_LOGWARN1(F("MISO:"), MISO);
  FTP_LOGWARN1(F("SCK:"),  SCK);
  FTP_LOGWARN1(F("SS:"),   SS);
  FTP_LOGWARN(F("========================="));
  
  // unknown board, do nothing, use default SS = 10
  #ifndef USE_THIS_SS_PIN
    #define USE_THIS_SS_PIN   10    // For other boards
  #endif

  #if defined(BOARD_NAME)
    FTP_LOGWARN3(F("Board :"), BOARD_NAME, F(", setCsPin:"), USE_THIS_SS_PIN);
  #else
    FTP_LOGWARN1(F("Unknown board setCsPin:"), USE_THIS_SS_PIN);
  #endif

  // For other boards, to change if necessary 
  Ethernet.init (USE_THIS_SS_PIN);

  // start the ethernet connection and the server:
  // Use DHCP dynamic IP and random mac
  uint16_t index = millis() % NUMBER_OF_MAC;
  // Use Static IP
  //Ethernet.begin(mac[index], ip);
  Ethernet.begin(mac[index]);

  Serial.print("IP Address = ");
  Serial.println(Ethernet.localIP());
  
#endif
}

void cardInit()
{
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect))
  {
    Serial.println("Initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  }
  else
  {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  
  switch (card.type())
  {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card))
  {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    
    return;
  }

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;

  Serial.print("\nVolume type is FAT"); Serial.println(volume.fatType(), DEC); Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters

  if (volumesize < 8388608ul)
  {
    Serial.print("Volume size (bytes): ");
    Serial.println(volumesize * 512);        // SD card blocks are always 512 bytes
  }

  Serial.print("Volume size (Kbytes): "); volumesize /= 2;    Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): "); volumesize /= 1024; Serial.println(volumesize);
}

void printDirectory(File dir, int numSpaces)
{
  while (true)
  {
    File entry = dir.openNextFile();

    if (! entry)
    {
      //Serial.println("** no more files **");
      break;
    }

    printSpaces(numSpaces);
    Serial.print(entry.name());

    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numSpaces + 2);
    }
    else
    {
      // files have sizes, directories do not
      unsigned int n = log10(entry.size());

      if (n > 10)
        n = 10;

      printSpaces(50 - numSpaces - strlen(entry.name()) - n);
      Serial.print("  "); Serial.print(entry.size(), DEC);
      
      DateTimeFields datetime;

      if (entry.getModifyTime(datetime))
      {
        printSpaces(4);
        printTime(datetime);
      }

      Serial.println();
    }
    
    entry.close();
  }
}

void printSpaces(int num)
{
  for (int i = 0; i < num; i++)
  {
    Serial.print(" ");
  }
}

void printTime(const DateTimeFields tm)
{
  const char *months[12] =
  {
      "January",  "February", "March",      "April",    "May",        "June",
      "July",     "August",   "September",  "October",  "November",   "December"
  };

  if (tm.hour < 10)
    Serial.print('0');

  Serial.print(tm.hour);
  Serial.print(':');

  if (tm.min < 10)
    Serial.print('0');

  Serial.print(tm.min);
  Serial.print("  ");
  Serial.print(months[tm.mon]);
  Serial.print(" ");
  Serial.print(tm.mday);
  Serial.print(", ");
  Serial.print(tm.year + 1900);
}

void SDCardTest()
{
  Serial.println("\n===============================");
  Serial.print("SDCard Initialization : ");

  
  if (!SD.begin(chipSelect))
  {
    Serial.println("failed!");
    return;
  }

  Serial.println("done.");

  File root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  delay(500);

  Serial.print(F("\nStarting FTP_Server_SDFAT2_NINA on ")); Serial.print(BOARD_NAME);
  Serial.print(F(" with ")); Serial.println(SHIELD_TYPE);
  Serial.println(FTP_SERVER_TEENSY41_VERSION);

// Uncomment this if Teensy 4.0 has SD card
#if (defined(ARDUINO_TEENSY41))

  Serial.println("Initializing SD card...");

  //////////////////////////////////////////////////////////////////

  cardInit();

  ////////////////////////////////////////////////////////////////

//  SDCardTest();

  //////////////////////////////////////////////////////////////////
#endif

  initNetwork();

  // Initialize the FTP server
  ftpSrv.init();
  ftpSrv.credentials( FTP_ACCOUNT, FTP_PASSWORD );

  Serial.print("FTP Server Credentials => account = "); Serial.print(FTP_ACCOUNT);
  Serial.print(", password = "); Serial.println(FTP_PASSWORD);
}

/*******************************************************************************
**                                                                            **
**                                 MAIN LOOP                                  **
**                                                                            **
*******************************************************************************/

void loop()
{
  ftpSrv.service();

  // more processes...
}
