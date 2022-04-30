# FTP_Server_Teensy41

[![arduino-library-badge](https://www.ardu-badge.com/badge/FTP_Server_Teensy41.svg?)](https://www.ardu-badge.com/FTP_Server_Teensy41)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/FTP_Server_Teensy41.svg)](https://github.com/khoih-prog/FTP_Server_Teensy41/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/FTP_Server_Teensy41.svg)](http://github.com/khoih-prog/FTP_Server_Teensy41/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of contents

* [Table of contents](#table-of-contents)
* [Why do we need this FTP_Server_Teensy41 library](#why-do-we-need-this-FTP_Server_Teensy41-library)
  * [Features](#features)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Teensy boards](#1-for-teensy-boards)
* [Original Documents](#Original-Documents) 
* [Examples](#examples)
  * [ 1. FTP_Server_SDFAT2](examples/FTP_Server_SDFAT2)
  * [ 2. multiFileProject](examples/multiFileProject)
* [Example FTP_Server_SDFAT2](#example-FTP_Server_SDFAT2)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. FTP_Server_SDFAT2 on Teensy4.1 QNEthernet](#1-FTP_Server_SDFAT2-on-Teensy41-QNEthernet)
  * [2. FTP_Server_SDFAT2 on Teensy4.1 NativeEthernet](#2-FTP_Server_SDFAT2-on-Teensy41-NativeEthernet)
  * [3. FTP_Server_SDFAT2 on TEENSY 4.0 with W5x00 using Ethernet_Generic Library](#3-FTP_Server_SDFAT2-on-TEENSY-40-with-W5x00-using-Ethernet_Generic-Library)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---


### Why do we need this [FTP_Server_Teensy41 library](https://github.com/khoih-prog/FTP_Server_Teensy41)

#### Features

This [**FTP_Server_Teensy41 library**](https://github.com/khoih-prog/FTP_Server_Teensy41) is modified from [Arduino-Ftp-Server library](https://github.com/gallegojm/Arduino-Ftp-Server) to support Teensy 4.x with `QNEthernet`, `NativeEthernet` or `W5x00 using Ethernet_Generic Library`, and currently SDFat2. In the near future, `any FS, such as PSRAM, S, (Q)SPI Flash, etc.` will be supported.

---

#### Currently supported Boards

1. **Teensy 4.1 using QNEthernet, NativeEthernet or W5x00 with Ethernet_Generic Library** and built-in SDCard, etc.
2. **Teensy 4.0 using W5x00 with Ethernet_Generic Library**

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Teensy core v1.56+`](https://www.pjrc.com/teensy/td_download.html) for Teensy 4.1
 3. [`QNEthernet Library version v0.14.0+`](https://github.com/ssilverman/QNEthernet) for Teensy 4.1 built-in Ethernet.
 4. [`Ethernet_Generic library v2.1.0+`](https://github.com/khoih-prog/Ethernet_Generic) to use **W5x00**. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/Ethernet_Generic.svg?)](https://www.ardu-badge.com/Ethernet_Generic).

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `FTP_Server_Teensy41`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/FTP_Server_Teensy41.svg?)](https://www.ardu-badge.com/FTP_Server_Teensy41) for more detailed instructions.

### Manual Install

1. Navigate to [FTP_Server_Teensy41](https://github.com/khoih-prog/FTP_Server_Teensy41) page.
2. Download the latest release `FTP_Server_Teensy41-master.zip`.
3. Extract the zip file to `FTP_Server_Teensy41-master` directory 
4. Copy the whole `FTP_Server_Teensy41-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**FTP_Server_Teensy41** library](https://registry.platformio.org/libraries/khoih-prog/FTP_Server_Teensy41) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/FTP_Server_Teensy41/installation). Search for **FTP_Server_Teensy41** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

#### 1. For Teensy boards
 
 **To be able to compile and run on Teensy boards**, you have to copy the files in [**Packages_Patches for Teensy directory**](Packages_Patches/hardware/teensy/avr) into Teensy hardware directory (./arduino-1.8.19/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.19. These files must be copied into the directory:

- `./arduino-1.8.19/hardware/teensy/avr/boards.txt`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-1.8.19/hardware/teensy/avr/cores/teensy4/Stream.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy4/Stream.h`

---
---

### Original Documents

Please have a look at [README_orig.md](README_orig.md) or [README.md](https://github.com/gallegojm/Arduino-Ftp-Server/blob/master/README.md)


---
---

### Examples

 1. [FTP_Server_SDFAT2](examples/FTP_Server_SDFAT2)
 2. [multiFileProject](examples/multiFileProject) 

---
---

### Example [FTP_Server_SDFAT2](examples/FTP_Server_SDFAT2)

#### 1. File [FTP_Server_SDFAT2.ino](examples/FTP_Server_SDFAT2/FTP_Server_SDFAT2.ino)

https://github.com/khoih-prog/FTP_Server_Teensy41/blob/5916a2c287b19b899eae9456c5937f522a8d6e30/examples/FTP_Server_SDFAT2/FTP_Server_SDFAT2.ino#L10-L369


---

#### 2. File [defines.h](examples/FTP_Server_SDFAT2/defines.h)

https://github.com/khoih-prog/FTP_Server_Teensy41/blob/5916a2c287b19b899eae9456c5937f522a8d6e30/examples/FTP_Server_SDFAT2/defines.h#L10-L126

---
---

### Debug Terminal Output Samples

#### 1. FTP_Server_SDFAT2 on TEENSY 4.1 with using QNEthernet

Following is debug terminal output when running example [FTP_Server_SDFAT2](examples/FTP_Server_SDFAT2) on Teensy4.1 using Built-in Ethernet and QNEthernet Library demonstrate the operation of FTP Server and SD Card using SDFat2.


```
Starting FTP_Server_SDFAT2 on TEENSY 4.1 with QNEthernet
FTP_Server_Teensy41 v1.0.0
Initializing SD card...
Wiring is correct and a card is present.

Card type: SDHC

Volume type is FAT32

Volume size (Kbytes): 31452672
Volume size (Mbytes): 30715

===============================
SDCard Initialization : done.
test.txt                                      1048702    22:42  April 29, 2022
foo.txt                                            13    00:00  January 1, 1980
datalog.bin                                  41943040    00:00  January 1, 2022
index.htm                                        3810    22:50  April 29, 2022
mydatalog.txt                                      10    22:24  April 9, 2022
edit.htm.gz                                      4116    20:15  March 13, 2022
CanadaFlag_1.png                                41214    20:15  March 13, 2022
CanadaFlag_2.png                                 8311    20:15  March 13, 2022
CanadaFlag_3.jpg                                11156    20:15  March 13, 2022
favicon.ico                                      1150    22:54  April 29, 2022
graphs.js.gz                                     1971    20:15  March 13, 2022
CanadaFlag_2_1.png                               8311    22:52  April 29, 2022
ESP_AT_WM_Lite.txt                               1547    22:51  April 29, 2022
done!

=========== USE_QN_ETHERNET ===========
Initialize Ethernet using static IP => IP Address = 192.168.2.241
FTP Server Credentials => account = teensy4x, password = ftp_test
[FTP]  Ftp server waiting for connection on port  21
[FTP]  Client connected!
USER teensy4x
PASS ftp_test
[FTP]  Authentication Ok. Waiting for commands.
SYST
DELE test.txt
PORT 192,168,2,30,169,133
[FTP]  Data IP set to  192.168.2.30 , Data port set to  43397
LIST
DELE datalog.bin
PORT 192,168,2,30,135,195
[FTP]  Data IP set to  192.168.2.30 , Data port set to  34755
LIST
PORT 192,168,2,30,165,55
[FTP]  Data IP set to  192.168.2.30 , Data port set to  42295
LIST
PORT 192,168,2,30,206,193
[FTP]  Data IP set to  192.168.2.30 , Data port set to  52929
RETR index.htm
[FTP]  Sending  index.htm
PORT 192,168,2,30,182,29
[FTP]  Data IP set to  192.168.2.30 , Data port set to  46621
STOR index1.htm
[FTP]  Receiving  index1.htm
[FTP]  Transfer completed in (ms)  250 , Speed (kbytes/s)  15
QUIT
[FTP]  Disconnecting client
[FTP]  Ftp server waiting for connection on port  21
```

---


#### 2. FTP_Server_SDFAT2 on TEENSY 4.1 with NativeEthernet

Following is debug terminal output when running example [FTP_Server_SDFAT2](examples/FTP_Server_SDFAT2) on Teensy4.1 using Built-in Ethernet and NativeEthernet Library demonstrate the operation of FTP Server and SD Card using SDFat2.

```
Starting FTP_Server_SDFAT2 on TEENSY 4.1 with NativeEthernet
FTP_Server_Teensy41 v1.0.0
Initializing SD card...
Wiring is correct and a card is present.

Card type: SDHC

Volume type is FAT32

Volume size (Kbytes): 31452672
Volume size (Mbytes): 30715

===============================
SDCard Initialization : done.
index1.htm                                       3810    23:11  April 29, 2022
foo.txt                                            13    00:00  January 1, 1980
index.htm                                        3810    22:50  April 29, 2022
mydatalog.txt                                      10    22:24  April 9, 2022
edit.htm.gz                                      4116    20:15  March 13, 2022
CanadaFlag_1.png                                41214    20:15  March 13, 2022
CanadaFlag_2.png                                 8311    20:15  March 13, 2022
CanadaFlag_3.jpg                                11156    20:15  March 13, 2022
favicon.ico                                      1150    22:54  April 29, 2022
graphs.js.gz                                     1971    20:15  March 13, 2022
CanadaFlag_2_1.png                               8311    22:52  April 29, 2022
ESP_AT_WM_Lite.txt                               1547    22:51  April 29, 2022
done!
======== USE_NATIVE_ETHERNET ========
Using mac index = 7
Connected! IP address: 192.168.2.103
FTP Server Credentials => account = teensy4x, password = ftp_test
[FTP]  Ftp server waiting for connection on port  21
[FTP]  Client connected!
USER teensy4x
PASS ftp_test
[FTP]  Authentication Ok. Waiting for commands.
SYST
PORT 192,168,2,30,146,13
[FTP]  Data IP set to  192.168.2.30 , Data port set to  37389
LIST
DELE index1.htm
PORT 192,168,2,30,170,27
[FTP]  Data IP set to  192.168.2.30 , Data port set to  43547
STOR index1.htm
[FTP]  Receiving  index1.htm
[FTP]  Transfer completed in (ms)  2 , Speed (kbytes/s)  1905
QUIT
[FTP]  Disconnecting client
[FTP]  Ftp server waiting for connection on port  21
```

---


#### 3. FTP_Server_SDFAT2 on TEENSY 4.0 with W5x00 using Ethernet_Generic Library with Large Buffer

Following is debug terminal output when running example [FTP_Server_SDFAT2](examples/FTP_Server_SDFAT2) on Teensy4.0 using W5x00 using Ethernet_Generic Library with Large Buffer.

```
Starting FTP_Server_SDFAT2 on TEENSY 4.0 with W5x00 using Ethernet_Generic Library with Large Buffer
FTP_Server_Teensy41 v1.0.0
======== USE_ETHERNET_GENERIC ========
[FTP] Default SPI pinout:
[FTP] MOSI: 11
[FTP] MISO: 12
[FTP] SCK: 13
[FTP] SS: 10
[FTP] =========================
[FTP] Board : TEENSY 4.0 , setCsPin: 10
IP Address = 192.168.2.118
FTP Server Credentials => account = teensy4x, password = ftp_test
[FTP]  Ftp server waiting for connection on port  21
[FTP]  Client connected!
USER teensy4x
PASS ftp_test
[FTP]  Authentication Ok. Waiting for commands.
SYST
PORT 192,168,2,30,233,47
[FTP]  Data IP set to  192.168.2.30 , Data port set to  59695
LIST
PORT 192,168,2,30,148,131
[FTP]  Data IP set to  192.168.2.30 , Data port set to  38019
```

---
---

### Debug

Debug is enabled by default on Serial. Debug Level from 0 to 4. To disable, change the _AWS_TEENSY41_LOGLEVEL_ to 0

```cpp
// Use this to output debug msgs to Serial
#define TEENSY41_DEBUG_PORT                 Serial

// Debug Level from 0 to 4
#define _FTP_SERVER_LOGLEVEL_               2
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of Arduino IDE, the Arduino `Teensyduino` core or depending libraries.

Sometimes, the library will only work if you update the `Teensyduino` core to the latest version because I'm always using the latest cores /libraries.


---

### Issues ###

Submit issues to: [FTP_Server_Teensy41 issues](https://github.com/khoih-prog/FTP_Server_Teensy41/issues)

---
---

## TO DO

 1. Fix bug. Add enhancement
 2. Add support to to `LittleFS`, (Q)SPI Flash, etc. besides SD card.


## DONE

 1. Initial porting and coding for **Teensy 4.1 using built-in QNEthernet, NativeEthernet or W5x00 using Ethernet_Generic Library**
 2. Support SD card and SDFat1, SDFat2
 3. Add debugging features.
 4. Add Table-of-Contents and Version String
 

---
---


### Contributions and Thanks

1. Based on and modified from [Jean-Michel Gallego's Arduino-Ftp-Server library](https://github.com/gallegojm/Arduino-Ftp-Server)


<table>
  <tr>
    <td align="center"><a href="https://github.com/gallegojm"><img src="https://github.com/gallegojm.png" width="100px;" alt="gallegojm"/><br /><sub><b>⭐️ Jean-Michel Gallego</b></sub></a><br /></td>
  </tr>
</table>

---

### Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [GPLv3](https://github.com/khoih-prog/FTP_Server_Teensy41/blob/main/LICENSE)

---

## Copyright

- Copyright 2014- Jean-Michel Gallego
- Copyright 2022- Khoi Hoang



