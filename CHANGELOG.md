# WisBlock-API-V2
----
Arduino library for RAKWireless WisBlock Core modules that takes all the LoRaWAN, BLE, AT command stuff off your workload. You concentrate on your application and leave the rest to the API. It is made as a companion to the [SX126x-Arduino LoRaWAN library](https://github.com/beegee-tokyo/SX126x-Arduino)    

# Release Notes

## 2.0.28 Add RAK3401/RAK3401
  - Add RAK3401/RAK3401 support for RAK13300 and RAK13302 LoRa transceiver modules 
  # _IMPORTANT:_
  _**RAK3400/RAK3401 support works only when using PlatformIO. ArduinoIDE is not supported at the moment**_
  
## 2.0.27 Fix AT+JOIN
  - Add command without parameter to just join

## 2.0.26 Fix RAK3112 BLE LED control
  - Fix the problems with missing BLE LED control on the RAK3112

## 2.0.25 Fix RAK3112 BLE
  - Fix the problems with AT commands over BLE on the RAK3112
  
## 2.0.24 Fix AT commands
  - Return AT_COMMAND_NOT_FOUND for unknown AT commands

## 2.0.23 Update
  - Add RAK3112 Core module with ESP32-S3 and integrated SX1262 transceiver
  
## 2.0.22 Improve RUI3 AT command compatibility
  - Add AT+FIRMWAREVER command
  - Add AT+SYNCWORD command
  
## 2.0.19 AT command parsing and default fPort changes
  - Allow upper and lower case and special characters in AT commands after the "="
  - Change LoRaWAN default fPort from 0 to 1

## 2.0.18 Fix dependency to CayenneLPP
  - Ownership of library has changed, PIO couldn't find the library
  
## 2.0.17 Rework AT commands
  - Make AT+PBW and AT+P2P compatible with WisToolBox

## 2.0.16 Add Arduino-Pico support
  - Support RAK11300 with Arduino-Pico BSP

## 2.0.15 Fix ESP32
  - Make ESP32 work with WisBlock-API-V2

## 2.0.14 Fix AT+PRECV command
  - Set RX_CONTINOUS depending on receive mode and re-initialize the LoRa transceiver 
  
## 2.0.13 New feature/bug-fix
  - Fix missing return value for ATC commands
  - Add option to re-init LoRaWAN stack

## 2.0.12  New features & Fix more compatibility problems with latest WisToolBox version
  - Give correct ATC+.... return instead of only AT+... for custom AT commands
  - Add AT command for force OTA DFU mode on RAK4631
  - Add re-initialize function for LoRaWAN (fix problem with confirmed msg NAK)

## 2.0.11 Smaller problem fixes
  - Add g_rx_fin_result = false; for LoRa P2P and call TX callback if CAD returns channel activity found
  - Fix compatibility problem with latest WisToolBox version by changeing AT error return values from +CME... to the new error strings

## 2.0.10 Enhance AT commands
  - Add option to set device alias

## 2.0.9 Enhancements
  - Enhance AT+BOOT to work with RP2040
  - Add option for custom boards (no LED's no BLE advertising)
  
## 2.0.8 Add AT+BOOT command to force DFU/UF2 bootloader mode

## 2.0.7 Add new CayenneLPP type for device ID (for Hummingbird PoC)

## 2.0.6 Some clean-up and improvement (e.g. no hang if LoRa init fails)

## 2.0.5 Fix new AT command
  - Fix wrong implementation of AT+PORT command

## 2.0.4 Add AT command
  - Add AT command to change fPort when using LoRaWAN. Thanks to @xoseperez

## 2.0.3 Fix value overflow
  - Fix value overflow in AT command AT+MASK

## 2.0.2 Fix P2P bandwidths
  - Fix wrong mapping for RUI3 bandwidths to WisBlock API bandwidths
  - Change status output (separate LoRaWAN and P2P)
  
## 2.0.1 Rename include file
  - Rename include file to avoid conflict with old version of WisBlock API
  
## 2.0.0 Make WisBlock API compatible to the RAKwireless RUI3 AT command interface
  - Create as new library WisBlock API V2 as it is not compatible with RAK11200 and RAK11310
  - Rework the AT command to be compatible with [RAKwireless RUI3](https://docs.rakwireless.com/RUI3)
  - Keep backwards compatibility to V1.1.18
  - Tested with desktop version of [RAKwireless WisToolBox](https://docs.rakwireless.com/Product-Categories/Software-Tools/WisToolBox/Overview/)
  - Not yet working with mobile version of WisToolBox (Device sync fails, but BLE UART terminal can be used already)

## 1.1.18 Add Cayenne LPP and cleanup AT commands
  - Add WisBlock extended Cayenne LPP to make it easier to use from examples
  - Replace AT command SENDFREQ with SENDINT to make it's meaning easier to understand (use word interval instead of frequency)

## 1.1.17 Add RAK11200 support and fix AT command manual
  - Add experimental support for RAK11200    
  - Correct AT command manual    
  
## 1.1.16 Small improvements
  - Make AT commands accept lower and upper case
  - AT+BAT=? returns battery level in volt instead of 0 - 254
  - BLE name changed to part of DevEUI for easier recognition of a device
  - Change ADC sampling time to 10 us for better accuracy

## 1.1.15 Remove external NV memory support
  - Removed support for external NV memory. Better to keep this on application level

## 1.1.14 External NV memory support
  - Add support for external NV memory. Prioritize usage of external NV memory over MCU flash memory.
  
## 1.1.13 Fix LoRa P2P bug
  - In LoRa P2P the automatic sending did not work because g_lpwan_has_joined stayed on false.
  
## 1.1.12 Fix timer bug
  - If timer is restarted with a new time, there was a bug that actually stopped the timer
  
## V1.1.11 Fix long sleep problem
  - Define alternate pdMS_TO_TICKS that casts uint64_t for long intervals due to limitation in nrf52840 BSP
  - Switch from using SoftwareTimer for nRF52 to using xTimer due to above problem
  - Change handling of user AT commands for more flexibility

## V1.1.10 P2P bug fix
  - If auto join for LPWAN is disabled, LoRa P2P mode is not working. Fixed.
  - Change response of AT+P2P=? to show bandwidth instead of index number
  - Thanks at @kongduino for finding both problems.

## V1.1.9 Remove sleep limitation
  - Send frequency was limited to 3600 seconds, set it to unlimited
  
## V1.1.8 Add more API functions
  - Add more API functions and update README

## V1.1.7 Add experimental support for RAK11310
  - Move more Core specific functions into the API functions
  - API functions support RAK4631 and RAK11310
  
## V1.1.6 Updated custom AT command handling
  - Make it easier to use custom AT commands and list them with AT?
  
## V1.1.5 Remove endless loop if app_init failed
  - Instead of endless loop, leave error handling to the application
  
## V1.1.4 Fix LoRa P2P
  - Fix bugs in LoRa P2P mode
  
## V1.1.3 Fix BLE MTU size
  - Fix bug with BLE MTU size to support extended LoRaWAN config structure
  
## V1.1.2 Add LoRa P2P support
  - Experimental support for LoRa P2P communication
  - Add support for user defined AT commands
  - Add example for LoRa P2P usage
  
## V1.1.1 Add library dependencies
  - Add SX126x-Arduino library dependency
  
## V1.1.0 Bug fix

## V1.0.9 Fix AT+STATUS command
  - Bugfix
  
## V1.0.8 Fix Compiler warnings
  - Bugfix
  
## V1.0.7 Make AT+NJM compatible with RAK3172/RUI AT commands
  - Adjust AT commands
  
## V1.0.6 Add AT+STATUS and AT+SEND commands
  - Additional AT commands
  
## V1.0.5 Make AT+JOIN conform with RAKwireless AT Command syntax
  - Change separator in AT+JOIN from _**`,`**_ to _**`:`**_
  
## V1.0.4 Fix AT command bugs
  - Fixed bug in AT+DR and AT+TXP commands

## V1.0.3 Add functionality
  - Fix AT command response delay
  - Add AT+MASK command to change channel settings for AU915, EU433 and US915
  - Added `api_read_credentials()`
  - `api_set_credentials()` saves to flash
  - Updated examples

## V1.0.2 Cleanup 
- Change debug output to generic `WisBlock API LoRaWAN` instead of GNSS

## V1.0.1 Improve examples
- Add missing LORA_JOIN_FIN handling in WisBlock-Kit-2 example

## V1.0.0 First release
  - Supports only WisBlock RAK4631 Core Module
