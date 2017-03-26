/*
KSM111 ESP8266 Module

FILE: KSM111_ESP8266.cpp
VERSION: 0.1.0
PURPOSE: KSM111 ESP8266 library for Arduino
LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

WEBSITE: 
www.kaise.com.tw
www.buyic.com.tw

*/


#ifndef KSM111_ESP8266_h
#define KSM111_ESP8266_h

#include <Arduino.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <Stream.h>
#include <SoftwareSerial.h>

#define STATION 1
#define AP 2
#define STATION_AP 3

#define SINGLE 0
#define MULTIPLE 1

#define OPEN	0
#define WEP		1
#define WPA_PSK 2
#define WPA2_PSK 3
#define WPA_WPA2_PSK 4		


class KSM111_ESP8266 : public SoftwareSerial
{
	public:
	KSM111_ESP8266(int,int,int);
	void begin(long);
	void hardreset();
	void softreset();
	void setmode(unsigned char mode);
	void listAP();
	void AT_CWJAP_L();
	bool disconnectAP();
	bool connectAP(String ap,String pwd);
	bool Res_OK();
	bool change_baud_rate(long baud);
	bool multipleconnect(unsigned char mul);
	String ShowIP(unsigned char mode);
	bool startclient(String type,String add,String port);
	bool startclient(unsigned char id,String type,String add,String port);
	bool startserver(String port);
	bool stopserver();
	bool stop();
	bool stop(unsigned char id);
	String ipstatus();
	bool setAP(String ssid,String pwd,unsigned char ch,unsigned char ecn);
	bool print(String str);
	bool print(unsigned char id,String str);
	bool println(String str);
	bool println(unsigned char id,String str);
	bool write(unsigned char id,char *data,int len);
	bool write_HTML(unsigned char id,char *data);
	bool send(unsigned char id,int len);
	
	String readstringdata();
	unsigned char currentID;
	
	void write(char c);
	void flush();
	int read();
	int available();
	bool find(char* str);
	
	
	
	void send_cmd(String);
	void (*Event_debug)(String data);
	void (*Event_list)(String data);
	void debug(String);
	

};

#endif 

