# Temperature Sensor

A simple device that saves measured temperature on a micro SD card once for every 5 minutes with some date from RTC. 

Electronics used:
* LM35 temperature sensor
* DS1302 RTC module
* SD card reader
* Arduino Nano

Connections:
* LM35 temperature sensor
  * 18 (A4)
* DS1302 RTC module
  * RST – 6
  * DAT – 7
  * CLK – 8
* SD card reader
  * SCK – 13
  * MISO – 12
  * MOSI – 11
  * CS – 9

  RTC library used: [RTC Virtuabotix Library by chrisfryer78](https://github.com/chrisfryer78/ArduinoRTClibrary)
