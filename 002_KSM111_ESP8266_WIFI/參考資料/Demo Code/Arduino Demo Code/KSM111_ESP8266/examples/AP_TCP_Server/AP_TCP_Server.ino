/*
AP Mode TCP SERVER

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

#include<KSM111_ESP8266.h>
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
#define LED 13
#define WIFIBaudRate 9600

String ssid="BuyIC";
String pwd ="12345678";
String port="8000";

KSM111_ESP8266 wifi(KSRobot_WIFI_RX,KSRobot_WIFI_TX,CE_PIN);

void print_debug(String data)
{
  Serial.print(data);
}

void setup()
{
  Serial.begin(9600);
  wifi.begin(WIFIBaudRate);
  Serial.println("KSM111_ESP8226");
  wifi.Event_debug = print_debug; // Add function print debug
  wifi.softreset();
  wifi.setmode(STATION_AP);
  if(wifi.setAP(ssid,pwd,1,WPA_WPA2_PSK))
  {
    wifi.multipleconnect(MULTIPLE);
    wifi.startserver(port);
    String ip = wifi.ShowIP(AP);
  } 
  
}
void loop()
{  
  
 /* if (wifi.available())
    Serial.write(wifi.read());
  if (Serial.available())
    wifi.write(Serial.read());
  */
  if (wifi.available())
  {
    if(wifi.find("CONNECT"))
    {
      while(!wifi.find("CLOSED"))
      {
        if(!wifi.println(0,"Send from KSM111_ESP8226"))
        {
          break;
        }
        delay(500);
      }
      wifi.debug("Connection closed");
    }
  } 
 
}
