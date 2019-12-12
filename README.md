# bitcoin-price-check
Arduino project to check and display bitcoin price

## Install Arduino

https://www.arduino.cc/en/Guide/HomePage

## Add Libraries

*Sketch->Include Library->Manage Libraries...* (see https://www.arduino.cc/en/guide/libraries)

* NTPClient by Fabrice Weinberg
* Timezone by Jack Christensen
* ArduinoJson by Benoit Blanchon
* GxEPD2 by Jean-Marc Zingg
* IotWebConf by Balazs Kelemen

## Install USB to UART Driver

In order to get bitcoin-price-check onto your ESP32 dev board you need to send the compiled code from your computer to the ESP32. If you computer doesn't have the necessary drivers, it won't recognize the device when you plug it into your USB port.

Assuming you have the same usb/uart controller I use, you'll need the driver from Silicon Labs. To check if you already have it, plug your device in the USB port. Wait about 10 seconds and type the command

`ls /dev/cu.*`

You should see a device the looks something like this, "/dev/cu.SLAB_USBtoUART". If you don't have it, the Silicon Labs USB to UART Bridge VCP driver located here are probably what you're looking for. (https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) Download, install and check for the device again as described above.

# Using Bitcoin Price Checker

## Features
* Displays the bitcoin price every 15 minutes from coindesk.com
* Uses e-paper and goes into deep sleep mode after retrieving the price
* Will become a WiFi access point to configure WiFi settings if it can’t connect to WiFi
* Access point SSID & password plus URL are displayed for easy config

## Known bugs / Opportunities for improvement
* Time zone is hard coded
* Access point SSID and Password are hardcoded and can’t be changed even though the config page makes you believe you can change it (ignore them)
* Connecting to the Bitcoin Price Checker’s access point sometimes fails and needs to be retired

## Setup Instructions
* Connect to power
* Wait for SSID, Pwd and URL to appear on the screen after WiFi connect times out
* Connect to the Bitcoin Price Checker’s access point
  * Connect iPhone, iPad or computer to WiFi Network BitcoinPrice
    * Use Password: ToTheMoon
  * If you are not redirected to the config screen go to URL: http://192.168.4.1/config
  * If the connection fails, wait a few seconds and try again
* Tell the Bitcoin Price Checker the WiFi credentials it should use to connect to the interwebs
  * Ignore Thing Name & AP password
    * If you see a message that you must enter an AP password, enter any random 8 characters
  * Enter the WiFi SSID the Bitcoin Price Checker will use to connect to the interwebs
  * Enter the WiFi password
* Obsess over bitcoin’s price
