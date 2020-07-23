// #==================================================================#
// ‖ Author: Luis Alejandro Domínguez Bueno (LADBSoft)                ‖
// ‖ Date: 2020-05-27                                    Version: 1.0 ‖
// #==================================================================#
// ‖ Name: ESP8266 MQTT doorbell simple notifier                      ‖
// ‖ Description: A sketch for the ESP8266 (ESP-01 to be exact) for   ‖
// ‖ adding IoT functionality to a classic doorbell, so you can send  ‖
// ‖ notifications using your IoT server, in this case, via MQTT.     ‖
// ‖                                                                  ‖
// ‖ The doorbell sensing is made using an optocoupler or similar     ‖
// ‖ circuit, by grounding <sensePin> pin.                          ‖
// #==================================================================#
// ‖ Version history:                                                 ‖
// #==================================================================#
// ‖ 1.0:  First stable version released.                             ‖
// #==================================================================#

// +------------------------------------------------------------------+
// |                        I N C L U D E S                           |
// +------------------------------------------------------------------+
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Configuration.h"

// +------------------------------------------------------------------+
// |                         G L O B A L S                            |
// +------------------------------------------------------------------+

WiFiClient espClient;
PubSubClient client(espClient);
long lastCheck = 0;
long lastTrueState = 0;
bool doorbellState = false;
bool isStateSent = false;

// +------------------------------------------------------------------+
// |                           S E T U P                              |
// +------------------------------------------------------------------+

void setup() {
  //Disable Serial pins in order to use them as GPIO
  pinMode(1, FUNCTION_3); //TX
  pinMode(3, FUNCTION_3); //RX

  pinMode(sensePin, INPUT_PULLUP); // For sensing the doorbell

  setup_wifi();
  client.setServer(mqttServer, mqttPort);
}

// +------------------------------------------------------------------+
// |                            L O O P                               |
// +------------------------------------------------------------------+

void loop() {
  //Ensure MQTT client is connected to server
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  //Doorbell ringing, but state not sent yet -> send state now
  if (doorbellState && !isStateSent) {
    publishState();
    isStateSent = true;
  }

  //After some time, reset doorbell state
  if (doorbellState && (now - lastTrueState) > 5000) {
    doorbellState = false;
    isStateSent = false;
  }

  //Periodic state broadcast
  if (now - lastCheck > 5000) {
    lastCheck = now;

    publishState();
  }
}

// +------------------------------------------------------------------+
// |                     S U B R O U T I N E S                        |
// +------------------------------------------------------------------+

void ICACHE_RAM_ATTR onDoorbellStateChange() {
  lastTrueState = millis();
  doorbellState = true;
}

void setup_wifi() {
  WiFiManager wifiManager;
  wifiManager.setTimeout(180); //3 minutes

  if(!wifiManager.autoConnect(wifiSsid, wifiPassword)) {
    //Retry after 3 minutes with no WiFi connection
    ESP.reset();
    delay(5000);
  }
}

void reconnect() {
  //There's no point on waiting for state changes while no connection to MQTT server is made
  detachInterrupt(digitalPinToInterrupt(sensePin));
  
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (!client.connect(mqttClientId, mqttUser, mqttPassword)) {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  //Continue waiting for state changes
  attachInterrupt(digitalPinToInterrupt(sensePin), onDoorbellStateChange, FALLING);
}

void publishState() {
//  Publish state
  if (doorbellState) {
    client.publish(mqttDoorbellTopic, "ON");
  } else {
    client.publish(mqttDoorbellTopic, "OFF");
  }
}
