# FTP_Server_Teensy41 Library


[![arduino-library-badge](https://www.ardu-badge.com/badge/FTP_Server_Teensy41.svg?)](https://www.ardu-badge.com/FTP_Server_Teensy41)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/FTP_Server_Teensy41.svg)](https://github.com/khoih-prog/FTP_Server_Teensy41/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/FTP_Server_Teensy41.svg)](http://github.com/khoih-prog/FTP_Server_Teensy41/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents


* [Changelog](#changelog)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.0](#releases-v100)

---
---

## Changelog

### Releases v1.2.0

1. Add support to `WiFiNINA`, such as `Adafruit Airlift Featherwing`
2. Configurable `user_name` length to max 63 and `user_password` to max 127. Check [Setting FTP_CRED_SIZE on the fly like with FTP_BUF_SIZE #4](https://github.com/khoih-prog/FTP_Server_Teensy41/issues/4)

### Releases v1.1.0

1. Fix bug incomplete downloads from server to client. Check [Incomplete downloads from server to client. #2](https://github.com/khoih-prog/FTP_Server_Teensy41/pull/2)


### Releases v1.0.0

1. Initial porting and coding for **Teensy 4.1 using built-in QNEthernet, NativeEthernet or W5x00 with Ethernet_Generic Library**
2. Currently supporting only SD Card. Will add support to LittleFS, etc. in next releases

