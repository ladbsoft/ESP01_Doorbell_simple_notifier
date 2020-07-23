# ESP01 MQTT Doorbell simple notifier
### A small project to notify when someone rings the doorbell

![Logo](https://raw.githubusercontent.com/ladbsoft/ESP01_Doorbell_simple_notifier/master/Logo.jpg "Logo")

## About this
Sometimes, I don't hear my doorbell ringing. Sometimes because I'm listening to music, because I'm using the vacuum cleaner, etc. So I thought notifications to my smartwatch should solve this, and hopefully this would help me not miss any Amazon parcels 😜...

The sketch uses MQTT to notify my MQTT server, which shoots the notification to my phone using Pushbullet, and [WiFiManager](https://github.com/tzapu/WiFiManager) to allow the WiFi configuration in case it changes without having to reprogram the board.

## Installation
You'll need:

### Hardware
- ESP-01 or similar microcontroller from the ESP8266 family
- A way of assembling the microcontroller to a small mains voltage detector and a small power supply. In my case, I used [this schematic](https://github.com/ladbsoft/ESP01_Doorbell_simple_notifier/blob/master/Schematic.zip)

### Software
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- The [ESP8266WiFi](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi) library
- The [PubSubClient](https://github.com/knolleary/pubsubclient) library
- The [ESP8266 WiFiManager](https://github.com/tzapu/WiFiManager) library
- An MQTT server to communicate to (in my case I used the embedded MQTT server from [OpenHAB](https://www.openhab.org/)

### Configuration of the Arduino Sketch
You'll need to configure in Configuration.h the IP, user, and password of your MQTT server, the client ID, and the MQTT topic to use and an SSID and password to create a new WiFi connection with a captive portal, that lets you connect to your WiFi router. This is not your actual WiFi router SSID and password, but a new one. Once you power on the microcontroller for the first time, connect to the SSID you configured here with your password and navigate to the captive portal to then configure your real WiFi connection.

More info (only in Spanish for now) in https://elprogramadoriberico.es/2020/07/23/avisador-de-timbre-casero/
