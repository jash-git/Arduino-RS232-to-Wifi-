#include "KSM111_ESP8266.h"
#include <SoftwareSerial.h>
//-----
#define KSRobot_WIFI_RX 2
#define KSRobot_WIFI_TX 3
#define CE_PIN   8//if use CE pin
//-----
KSM111_ESP8266 wifi(KSRobot_WIFI_RX,KSRobot_WIFI_TX,CE_PIN);//2,3,8

void setup() {
  // enable debug serial
  Serial.begin(9600); 
  // enable software serial
  wifi.begin(9600);
}

void loop() {
  if (wifi.available()) {
    Serial.write(wifi.read());
  }
  if (Serial.available()) {
    char chars = Serial.read();
    wifi.write(chars);
  } 
}
