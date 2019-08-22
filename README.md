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



