/*
Set Baudrate

KSM111 ESP8266 Module

VERSION: 0.1.0
PURPOSE: KSM111 ESP8266 library for Arduino
LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

WEBSITE: 
www.kaise.com.tw
www.buyic.com.tw

HISTORY:
0.92 and 0.95 myip
0.92 and 0.95 Link->CONNECT , UNLINK ->CLOSED


*/


#include "KSM111_ESP8266.h"
#include <SoftwareSerial.h>

#define UNO

#ifdef UNO 
#define KSRobot_WIFI_RX 2
#define KSRobot_WIFI_TX 3
#else
#define KSRobot_WIFI_RX 10
#define KSRobot_WIFI_TX 11
#endif

#define CE_PIN   8						//if use CE pin
#define WIFIBaudRate 115200
#define SETBaudRate 9600
#define LED 13


KSM111_ESP8266 wifi(KSRobot_WIFI_RX,KSRobot_WIFI_TX,CE_PIN);


void print_debug(String data)
{
  Serial.print(data);
}
void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  wifi.begin(WIFIBaudRate);
  Serial.println("KSM111_ESP8226");
  Serial.println("Set Baudrate 115200 to "+String(SETBaudRate));
  digitalWrite(LED,HIGH);
  delay(5000);
  wifi.Event_debug = print_debug;
  wifi.change_baud_rate(SETBaudRate);
  digitalWrite(LED,LOW);
  wifi.Event_list = print_debug;
 
}
void loop()
{

}
