  /*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */ 

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "thermalprinter.h"

Epson TM88 = Epson(14, 12);
SoftwareSerial mySerial(5, 4);

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

String request, req_final;

byte ledPin = 2;
char rcv_char;
String a, rcv_sms, defined_text;
unsigned long int inc=0, credit=0, total=0, balance=0;

void setup() {
  
  Serial.begin(9600);
  mySerial.begin (9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Wemos_AP", "Wemos_comm");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  pinMode(ledPin, OUTPUT);
  Serial.println();
  Serial.println("accesspoint_bare_01.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());

  delay (15000);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  delay(500); //updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(500); //updateSerial();  
  mySerial.println("AT+CNMI=2,2,0,0,0");
  delay(500); //updateSerial();

}

void loop() {

  WiFiClient client = server.available();
  
  if (!client) {

     if(mySerial.available()) {
      
      rcv_sms = mySerial.readString();//Forward what Software Serial received to Serial Port
      
      if (rcv_sms.substring(rcv_sms.indexOf("BILL"),rcv_sms.indexOf("BILL")+4) == "BILL") {
        
        mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
        delay(500); //updateSerial();
        mySerial.println("AT+CMGS=\"+923332140544\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
        delay(500); //updateSerial();
        mySerial.println(req_final);
        delay(500); //updateSerial();
        mySerial.write(26);  
        }

      //Serial.println (rcv_sms);
      rcv_sms = "";
     }
     
    
    if (Serial.available()) {
      
        a = Serial.readString();
  
        if (a.substring(0,1) == "a") {
  
          Serial.println(req_final); 
             
          }
        
        if (a.substring(0,3) == "cr=") {

          credit = a.substring(a.indexOf('=')+1).toInt();
          //crbalance = credit;
          Serial.print ("credit = ");
          Serial.println (credit);
        }

        if (a.substring(0,1) == "b") {
  
          //swSer.println(req_final);           
          TM88.start();
          TM88.justifyLeft();
          
          TM88.write(0x1B);    
          TM88.write(0x21);  
          TM88.write(32);
          
          TM88.println(req_final);

          TM88.feed(3);
      
          TM88.cut();
                 
//          for (int q=0; q<3; q++)  {
//            
//            swSer.println();  
//            }   
          }
      }
      a = "";
    return;
    }
  
  digitalWrite(ledPin, LOW);
  request = client.readStringUntil('\r');

  total = request.substring(request.indexOf("Total   :   ")+12).toInt();
  //Serial.println (total);
  balance = credit - total;  
  //Serial.println (balance);
  
  Serial.println("********************************");
  //Serial.println("From the station: " + request);

  req_final = "\nSMART SHOPPING SYSTEM\n\nTROLLEY : 001\n\nCredit  :  " + String(credit) + "\n\n" 
                  + request + "\nBalance :  " + String(balance) + "\n---------------------\n";
  
  Serial.println(req_final);

  client.flush();
  //Serial.print("Byte sent to the station: ");
  Serial.println(client.println(request + "ca" + "\r"));
  digitalWrite(ledPin, HIGH);

  TM88.start();

  TM88.justifyLeft();
  
  TM88.write(0x1B);    
  TM88.write(0x21);  
  TM88.write(32);

  TM88.println(req_final);

  TM88.feed(3);

  TM88.cut();
          
  credit = balance;
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
