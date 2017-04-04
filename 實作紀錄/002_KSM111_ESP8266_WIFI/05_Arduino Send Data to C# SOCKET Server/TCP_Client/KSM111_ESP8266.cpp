/*
KSM111 ESP8266 Module

FILE: KSM111_ESP8266.cpp
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


#ifdef __cplusplus
extern "C" 
{
#endif
  #include <inttypes.h>
  #include <avr/io.h>
  #include <avr/interrupt.h>
#ifdef __cplusplus
}
#endif
#if ARDUINO >= 100
#include "Arduino.h"
#else   
#include "WProgram.h"
#endif
#include "KSM111_ESP8266.h"

int enable_pin;
void event_null(String data){}
char str_data_in[100]="";
unsigned char cnt_str_data=0;
String data_out="";

KSM111_ESP8266 :: KSM111_ESP8266(int rx,int tx,int CE_PIN):SoftwareSerial(rx,tx)
{
	Event_debug =  event_null;
	Event_list = event_null;
	pinMode(CE_PIN, OUTPUT);
	enable_pin = CE_PIN;

}

void KSM111_ESP8266 :: begin(long baudrate)
{
	SoftwareSerial::begin(baudrate);
}

void KSM111_ESP8266 :: hardreset()
{
	char input;
	/*digitalWrite(enable_pin,LOW);   
	delay(1000);               
	digitalWrite(enable_pin,HIGH);
	delay(3000);	*/
	
	send_cmd("AT+RST");
	delay(5000);
	while (SoftwareSerial::available())
	{
		input = SoftwareSerial::read();
		//debug(String(input));
	}
	//delay(2000);
	//send_cmd("AT");
	//send_cmd("AT");
	debug("Ready\r\n");

}

void KSM111_ESP8266 :: softreset()
{
	char input;
	send_cmd("AT+RST");
	delay(5000);
	while (SoftwareSerial::available())
	{
		 //SoftwareSerial::read();
		input = SoftwareSerial::read();
		//debug(String(input));
	}
	//delay(2000);
	//send_cmd("AT");
	//send_cmd("AT");
	debug("Ready\r\n");
}

void KSM111_ESP8266 :: setmode(unsigned char mode)
{
	char input;
	SoftwareSerial::flush();
	String cmd = "AT+CWMODE=" + String(mode);
	debug("Setmode = ");
	send_cmd(cmd);
	delay(500);
	while (SoftwareSerial::available())
	{
		input = SoftwareSerial::read();
		debug(String(input));
	}
}

void KSM111_ESP8266 :: listAP()
{
	char input;
	char end[5];
	unsigned char cnt=0;
	SoftwareSerial::flush();
	send_cmd("AT+CWLAP");
	debug("List AP = ");
	long previousMillis = 0;
	long interval = 1000;
	unsigned char time=0;
	while(1)
	{
		while (SoftwareSerial::available())
		{
			input  = SoftwareSerial::read();
			(*Event_list)(String(input));
			
		}
		 unsigned long currentMillis = millis();
		 if(currentMillis - previousMillis > interval) 
		 {
			previousMillis = currentMillis;
			if(time++>5)
			{
				debug("\r\n");
				debug("<---END--->");
				debug("\r\n");
				break;
			}
		 }
	}	
	
}

void KSM111_ESP8266 :: AT_CWJAP_L()
{
	char input;
	char end[5];
	unsigned char cnt=0;
	SoftwareSerial::flush();
	send_cmd("AT+CWJAP?");
	long previousMillis = 0;
	long interval = 1000;
	unsigned char time=0;
	while(1)
	{
		while (SoftwareSerial::available())
		{
			input  = SoftwareSerial::read();
			(*Event_list)(String(input));
			
		}
		 unsigned long currentMillis = millis();
		 if(currentMillis - previousMillis > interval) 
		 {
			previousMillis = currentMillis;
			if(time++>5)
			{
				debug("\r\n");
				debug("<---END--->");
				debug("\r\n");
				break;
			}
		 }
	}	
	
}



bool KSM111_ESP8266 :: connectAP(String ap , String pwd)
{
	bool res=false;
	char input;
	char str[10];
	unsigned str_cnt=0;
	String cmd = "AT+CWJAP=" + String("\"") + ap + String("\"") + String(",")+String("\"") + pwd + String("\"");
	send_cmd(cmd);
	debug("Please wait 8s ...\r\n");
	debug("Connect AP =");
	
	delay(8000);
	while(1)
	{
		if(SoftwareSerial::available())
		{
			input = SoftwareSerial::read();
			if(input=='K')
			{
				res=true;
				debug("OK\r\n");
				break;
			}
			if(input=='F')
			{
				res = false;
				debug("Fail\r\n");
				break;
			}
		}
	}
	return(res);
}

bool KSM111_ESP8266::disconnectAP()
{
	SoftwareSerial::flush();
	send_cmd("AT+CWQAP");
	debug("Disconnect AP =");
	delay(100);
	return(Res_OK());
}

bool KSM111_ESP8266 :: multipleconnect(unsigned char mul)
{
	String cmd = "AT+CIPMUX=" + String(mul);
	SoftwareSerial::flush();
	send_cmd(cmd);
	debug("multipleconnect = ");
	delay(100);
	return(Res_OK());
}



bool KSM111_ESP8266:: startclient(String type,String add,String port)
{
	bool res=false;
	String cmd = "AT+CIPSTART="+String("\"")+type+String("\",")+String("\"")+add+String("\",")+port;
	debug("Start client = ");
	SoftwareSerial::flush();
	send_cmd(cmd);
	delay(100);
	char input;
	
	if (SoftwareSerial::find("CONNECT"))
	{
		res=true;
		debug("OK,CONNECT\r\n");
	}	
	else
	{
		res=false;
		debug("Fail,Unlink\r\n");
	}

	return(res);
}

bool KSM111_ESP8266:: startclient(unsigned char id,String type,String add,String port)
{
	bool res=false;
	String cmd = "AT+CIPSTART=" + String(id) + "," + String("\"")+type+String("\",")+String("\"")+add+String("\",")+port;
	debug("Srart client = ");
	SoftwareSerial::flush();
	send_cmd(cmd);
	delay(100);
	char input;
	
	if ( SoftwareSerial::find("CONNECT"))
	{
		res=true;
		debug("OK,CONNECT\r\n");
	}	
	else
	{
		res=false;
		debug("Fail,Unlink\r\n");
	}

	return(res);
}

bool KSM111_ESP8266 :: startserver(String port)
{
	bool res=0;
	String cmd = "AT+CIPSERVER="+String("1")+String(",")+port;
	send_cmd(cmd);
	debug("Start Server = Port "+port+" ");
	delay(100);
	if (SoftwareSerial::find("OK"))
	{
		debug("OK\r\n");
		res = true;
	}
	else if (SoftwareSerial::find("no change"))
	{
		debug("no change\r\n");
		res = true;
	}
	else
	{
		debug("Error\r\n");
		res = false;
	}
	return(res);
	
}

bool KSM111_ESP8266 ::stopserver()
{
	bool res=0;
	String cmd = "AT+CIPSERVER="+String("0");
	send_cmd(cmd);
	debug("Stop Server ");
	delay(100);
	if (SoftwareSerial::find("OK"))
	{
		debug("OK\r\n");
		res = true;
	}
	else
	{
		debug("Error\r\n");
		res = false;
	}
	return(res);
	
}

bool KSM111_ESP8266 :: stop()
{
	bool res=false;
	send_cmd("AT+CIPCLOSE");
	debug("Stop = ");
	delay(100);
	if (SoftwareSerial::find("CLOSED"))
	{
		res=true;
		debug("OK,CLOSED\r\n");
	}	
	else
	{
		res=false;
		debug("ERROR\r\n");
	}
	
	return(res);
}

bool KSM111_ESP8266 :: stop(unsigned char id)
{
	bool res=false;
	String cmd = "AT+CIPCLOSE="+String(id);
	send_cmd(cmd);
	debug("Stop = ");
	delay(100);
	if (SoftwareSerial::find("OK"))
	{
		res=true;
		debug("OK,Unlink\r\n");
	}	
	else
	{
		res=false;
		debug("ERROR\r\n");
	}
	
	return(res);
}

bool KSM111_ESP8266 :: print(String str)
{
   bool res;
   String cmd = "AT+CIPSEND="+String(str.length());
   SoftwareSerial::flush();
   send_cmd(cmd);
   String buf="";
   if (SoftwareSerial::find(">"))
	{
		debug("Send -->");
		debug(str);
		debug("\r\n");
		res = true; 
	}	
	else
	{
		debug("Send Error\r\n");
		res = false;
	}
	SoftwareSerial::print(str);
        ///*
	if (SoftwareSerial::find("SEND OK"))
	{
		debug("SEND OK\r\n");
                Serial.print(buf);//--2017/04/04
		res = true;
	}
	else
	{
		debug("SEND Fail\r\n");
		res = false;
	}
        //*/
   return(res);
}

bool KSM111_ESP8266 :: print(unsigned char id,String str)
{
	String cmd = "AT+CIPSEND="+String(id)+String(",")+String(str.length());
    SoftwareSerial::flush();
    send_cmd(cmd);
   
   if (SoftwareSerial::find(">"))
	{
		debug("Send -->");
		debug(str);
		debug("\r\n");
	}	
	else
	{
		debug("Send Error\r\n");
	}
	SoftwareSerial::print(str);
	if (SoftwareSerial::find("SEND OK"))
	{
		debug("SEND OK\r\n");
	}
	else
	{
		debug("SEND Fail\r\n");
	}
}

bool KSM111_ESP8266 :: println(String str)
{
    String data = str+String("\r\n"); 
	return(print(data));
}

bool KSM111_ESP8266 :: println(unsigned char id,String str)
{
	String data = str+String("\r\n"); 
	return(print(id,data));
}

bool KSM111_ESP8266 :: send(unsigned char id,int len)
{
	bool res;
	String cmd = "AT+CIPSEND="+String(id)+String(",")+String(len);
	SoftwareSerial::flush();
    send_cmd(cmd);
   if (SoftwareSerial::find(">"))
	{
		debug("Send ...");
		debug("\r\n");
		res = true; 
	}	
	else
	{
		debug("Send Error\r\n");
		res = false;
	}
	return(res);
}

bool KSM111_ESP8266 :: write(unsigned char id,char *data,int len)
{
	bool res;
	
	res = send(id,len);
	if(!res)
	return(res);
	for(int i=0;i<len;i++)
	{
		SoftwareSerial::write(data[i]);
	}
	if (SoftwareSerial::find("SEND OK"))
	{
		debug("SEND OK\r\n");
		res = true;
	}
	else
	{
		debug("SEND Fail\r\n");
		res = false;
	}
   return(res);
	
}

bool KSM111_ESP8266 :: write_HTML(unsigned char id,char *data)
{
	int len=0;
	while(data[len])
	{
		len++;
	}
	write(id,data,len);
	
}

String KSM111_ESP8266 ::ipstatus()
{
	String status;
	char input;
	SoftwareSerial::flush();
	send_cmd("AT+CIPSTATUS");
	
	while (1)
	{
		if(SoftwareSerial::available())
		{
			input = SoftwareSerial::read();
			status += String(input);
			//debug(String(input));
			if(input=='K')
				break;
		}
		
	}
	debug(status);
return(status);
}

bool KSM111_ESP8266 :: setAP(String ssid,String pwd,unsigned char ch,unsigned char ecn)
{
	//flush();
	String cmd ="AT+CWSAP="+String("\"")+ssid+String("\",")+String("\"")+pwd+String("\",")+String(ch)+String(",")+String(ecn);
	send_cmd(cmd);
	debug(cmd);
	debug("\r\n");
	debug("Set Access Point =");
return(Res_OK());
}

int KSM111_ESP8266 :: read()
{
	return(SoftwareSerial::read());
}

int KSM111_ESP8266 :: available()
{
	return(SoftwareSerial::available());
}

bool KSM111_ESP8266 :: find(char* str)
{
	return(SoftwareSerial::find(str));
}

void KSM111_ESP8266 :: write(char c)
{
	SoftwareSerial::write(c);
}

void KSM111_ESP8266 :: flush()
{
	SoftwareSerial::flush();
	send_cmd("AT");
	send_cmd("AT");
}

String KSM111_ESP8266 :: readstringdata()
{
//+IPD,0,7:LED OFF
//char str_data_in[50]="";
//unsigned char cnt_str_data=0;
  while (SoftwareSerial::available())
  {
		char input = SoftwareSerial::read();
		//Serial.print(input);
		if((input != 0x0A)&&(input != 0x0D))
		{
			str_data_in[cnt_str_data] = input;
			cnt_str_data++;
			if(cnt_str_data>99)
			{	
				cnt_str_data=0;
			}
			if(str_data_in[0] != '+')
			{
				cnt_str_data=0;
			}
		}
		if((input == 0x0A)&& (str_data_in[0] == '+'))
		{
			str_data_in[cnt_str_data]=0;
			String data_str = String(str_data_in);
			unsigned char index = data_str.indexOf(':');
			currentID = str_data_in[5]-0x30;
			data_out = data_str.substring(index+1);
			//Serial.print(data_out);
			Serial.print("ID="+ String(currentID)+"\r\n");
			cnt_str_data=0;
			return(data_out);
		}
	}
	return("");
}

String KSM111_ESP8266 :: ShowIP(unsigned char mode)
{
  char input;
	String all_in="";
	unsigned char cnt=0;
	String my_ip="";
	SoftwareSerial::flush();
	if(mode == STATION)
		send_cmd("AT+CIPSTA?");
	else if(mode == AP)
		send_cmd("AT+CIPAP?");
	delay(500);
		while(1)
		{
			if(SoftwareSerial::available())
			{
				input = SoftwareSerial::read();
				all_in += String(input);
				//debug(String(input));
				if(((input >= 0x30)&&(input <= 0x39))||(input == '.'))
				{
					my_ip += String(input);
				}
				if(input=='K')
				{
					my_ip += String("\r\n");
				
				break;
			  }
			}
		}		
	
		
	if(my_ip.length()>2)
	{
		
	if(mode == STATION)
		debug("STA IP =");
	else if(mode == AP)
		debug("AP IP =");
		debug(my_ip);
		debug("\r\n");
	}
	else
	{
		debug(all_in);
		return("Error");
	}

   return(my_ip);

}

bool KSM111_ESP8266 :: change_baud_rate(long baud)
{
	bool res = false;
	String cmd = "AT+CIOBAUD="+String(baud);
	send_cmd(cmd);
	debug("Set Baudrate to "+String(baud)+"\r\n");
	SoftwareSerial::begin(baud);
	return(res);
}


void KSM111_ESP8266 :: send_cmd(String data)
{
	 SoftwareSerial::println(data);
}

void KSM111_ESP8266:: debug(String data)
{
   (*Event_debug)(data);
}

bool KSM111_ESP8266:: Res_OK()
{
	bool res=false;
	if ( SoftwareSerial::find("OK"))
	{
		debug("OK\r\n");
		res = true;
	}
	else
	{
		debug("Error\r\n");
		res = false;
	}
	return(res);
}



