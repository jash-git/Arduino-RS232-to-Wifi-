/*
STA Mode TCP CLIENT

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
#define LED 13
#define WIFIBaudRate 9600


KSM111_ESP8266 wifi(KSRobot_WIFI_RX,KSRobot_WIFI_TX,CE_PIN);

String ssid="jash.net-G-WEP";
String pass="jashasd700502";
String IP = "192.168.104.3";
String Path = "val.php?string=";
int value=100;




void print_debug(String data)
{
  Serial.print(data);
}
void setup()
{
  Serial.begin(9600);
  wifi.begin(WIFIBaudRate);
  Serial.println("KSM111_ESP8226");
  wifi.Event_debug = print_debug;
  wifi.softreset();
  wifi.setmode(STATION);
  wifi.disconnectAP();
  if( wifi.connectAP(ssid,pass))
  {
  
      wifi.multipleconnect(SINGLE);
      String ip = wifi.ShowIP(STATION);
      wifi.startclient("TCP",IP,"8080");
  }
}
void loop()
{

	// 準備要GET的字串
	// HTTP/1.1
        String cmd;
	String getstr = "GET /"+Path+String(value)+" HTTP/1.1\r\nHost:"+IP+":8080\r\n\r\n";//
	// 發送指定長度的數據
	cmd = "AT+CIPSEND="+String(getstr.length());
        //Serial.println("call api:"+cmd);
        wifi.println(getstr);
	wifi.println(cmd);
	//Serial.println("length: "+cmd);
	delay(1000);

  	// 長度的數據傳送成功，會返回 ">"，然後開始傳輸通訊埠數據
	if(wifi.find(">")){
		Serial.println("> "+getstr);
	}
	else{
		// 關閉連線
		wifi.println("AT+CIPCLOSE");
		Serial.println("AT+CIPCLOSE  connection closed!\r\n\r\n");
		delay(2000);
		return;
	} 
	wifi.println(getstr);
	delay(2000);

}

