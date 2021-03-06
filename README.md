# ESP-Sensor-Module
This is a repository containing code and schematics for a ESP8266 based sensor for home automation

## Revision 1
This is the first iteration of the code, where a user specifies the following at compile time:
  - SSID of network to connect to
  - PSK of network to connect to 
  - MQTT server IP address
  - Publish topic
  - Subscribe topic

Once compiled and run on the ESP8266, the program connects to the MQTT broker, and will publish "ON" to the publish topic when the PIR sensor is triggered, and "OFF" 5 seconds after the sensor is triggered. The program also notifies the user of successful/unsuccessful connection via an LED, as well as an audible tone. 

When a message of the format "BUZZER ON xxxx" is recieved on the subscribed topic, the speaker will produce a tone of xxxx Hz until a message of the format "BUZZER OFF" is recieved.  When a message of the format "BEEPER ON xxxx" is recieved on the subscribed topic, the speaker will produce beeping at xxxx Hz until a message of the format "BUZZER OFF" is recieved. 

##References
This code used the following for references, I would like to thank the original authors:
  - Arduino IDE examples for ArduinoOTA, ESP8266mDNS, DHT, ArduinoJson, and PubSubClient
  - https://home-assistant.io/blog/2015/10/11/measure-temperature-with-esp8266-and-report-to-mqtt/
