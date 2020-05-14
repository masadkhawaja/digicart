/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#include <ESP8266WiFi.h>

#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

IPAddress server(192,168,4,15);     // IP address of the AP
WiFiClient client;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

SoftwareSerial serial(14, 12, false, 256); // RX, TX

PN532_SWHSU pn532swhsu( serial );
PN532 nfc( pn532swhsu );

#define tx_button 13
#define clear_button 16
#define sub_button 15

boolean success;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

uint8_t card1[4] = {0xEA, 0x1D, 0x4E, 0x1F};
uint8_t card2[4] = {0xFA, 0x3B, 0x10, 0x1F};
uint8_t card3[4] = {0xEA, 0x3F, 0x93, 0x1F};
uint8_t card4[4] = {0xEA, 0x73, 0xB0, 0x1F};
uint8_t card5[4] = {0xEA, 0x46, 0x08, 0x1F};
uint8_t card6[4] = {0xEA, 0xAB, 0xB0, 0x1F};
uint8_t card7[4] = {0xEA, 0x48, 0xC4, 0x1F};
uint8_t card8[4] = {0x4B, 0x86, 0xFE, 0x19};
uint8_t card9[4] = {0xEA, 0x87, 0x86, 0x1F};
uint8_t card10[4] = {0x8D, 0xE5, 0xB5, 0x2D};

int product1 = 1, product2 = 2, product3 = 3, product4 = 4, product5 = 5, product6 = 6, product7 = 7, product8 = 8, product9 = 9, product10 = 10;

int total;

int check1=0, check2=0, check3=0, check4=0, check5=0, check6=0, subtract_check=0; 

char ssid[] = "Wemos_AP";           // SSID of your AP
char pass[] = "Wemos_comm";         // password of your AP

unsigned long int inc;

String P_1 = "Card 1", P_2 = "Card 1", P_3 = "Card 1", P_4 = "Card 5", P_5 = "Card 5";    
String P_6 = "Card 6", P_7 = "Card 7", P_8 = "Card 8", P_9 = "Card 9", P_10 = "Card 10";

String tx_str;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(sub_button, INPUT);
  pinMode(tx_button, INPUT);
  pinMode(clear_button, INPUT);
  
  Serial.begin(9600);
  serial.begin(115200);

  Serial.println("Hello!");

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" SMART TROLLY : 001");
  
  nfc.begin();
  
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
    
  Serial.println("Waiting for an ISO14443A card");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println("station_bare_01.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
}
// the loop function runs over and over again forever
void loop() {

  inc++;

  if (digitalRead(clear_button) && !check1) {

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print(" SMART TROLLY : 001");
    
    check2 = 0;
    check1 = 1;
    }

  if (!digitalRead(clear_button) && !check2) {  check1 = 0; check2 = 1; }

  if (digitalRead(tx_button) && !check3) {

    client.connect(server, 80);

    Serial.println("********************************");

    tx_str = tx_str + "---------------------\n" + "Total   :   " + total + "\n---------------------";

    Serial.println(client.print(tx_str));
    Serial.println(client.print("\r"));
    
    //String answer = client.readStringUntil('\r');
    //Serial.println("From the AP: " + answer);
    client.flush();
  //  digitalWrite(ledPin, HIGH);
    client.stop();

    total = 0;
    tx_str = "";
  
    delay (1000);
    
    check4 = 0;
    check3 = 1;
    }

  if (!digitalRead(tx_button) && !check4) {  check3 = 0; check4 = 1; }

  if (digitalRead(sub_button) && !check5) {

    subtract_check = 1;
    
    check6 = 0;
    check5 = 1;
    }

  if (!digitalRead(sub_button) && !check6) {  check5 = 0; check6 = 1; }
 
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  if (inc == 50000) {

    Serial.println("Timed out waiting for a card");
    
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    
    inc = 0;
    }
  
  if (success) {
    
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");

    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print("ID :");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");Serial.print(uid[i], HEX); 
      
      lcd.print(" ");
      lcd.print(uid[i], HEX);
    }

    lcd.setCursor(0,2); lcd.print("NAME :              "); lcd.setCursor(0,3); lcd.print("                    ");

    if (card1[0] == uid[0] && card1[1] == uid[1] && card1[2] == uid[2] && card1[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 1"); lcd.setCursor(0,3); lcd.print(product1);  
      
      if (!subtract_check) { total = total + product1; tx_str = tx_str + P_1 + "    :   " + product1 + "\n";} 
      if (subtract_check) { total = total - product1; tx_str = tx_str + P_1 + "    :  -" + product1 + "\n"; subtract_check=0; } }        
    
    if (card2[0] == uid[0] && card2[1] == uid[1] && card2[2] == uid[2] && card2[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 2"); lcd.setCursor(0,3); lcd.print(product2); 
                                                                                                              
      if (!subtract_check) { total = total + product2; tx_str = tx_str + P_2 + "    :   " + product2 + "\n";} 
      if (subtract_check) { total = total - product2; tx_str = tx_str + P_2 + "    :  -" + product2 + "\n"; subtract_check=0; } }
      
    if (card3[0] == uid[0] && card3[1] == uid[1] && card3[2] == uid[2] && card3[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 3"); lcd.setCursor(0,3); lcd.print(product3); 
      
      if (!subtract_check) { total = total + product3; tx_str = tx_str + P_3 + "    :   " + product3 + "\n";} 
      if (subtract_check) { total = total - product3; tx_str = tx_str + P_3 + "    :  -" + product3 + "\n"; subtract_check=0; } }
    
    if (card4[0] == uid[0] && card4[1] == uid[1] && card4[2] == uid[2] && card4[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 4"); lcd.setCursor(0,3); lcd.print(product4); 
      
      if (!subtract_check) { total = total + product4; tx_str = tx_str + P_4 + "    :   " + product4 + "\n";} 
      if (subtract_check) { total = total - product4; tx_str = tx_str + P_4 + "    :  -" + product4 + "\n"; subtract_check=0; } }
      
    if (card5[0] == uid[0] && card5[1] == uid[1] && card5[2] == uid[2] && card5[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 5"); lcd.setCursor(0,3); lcd.print(product5); 
      
      if (!subtract_check) { total = total + product5; tx_str = tx_str + P_5 + "    :   " + product5 + "\n";} 
      if (subtract_check) { total = total - product5; tx_str = tx_str + P_5 + "    :  -" + product5 + "\n"; subtract_check=0; } }
    
    if (card6[0] == uid[0] && card6[1] == uid[1] && card6[2] == uid[2] && card6[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 6"); lcd.setCursor(0,3); lcd.print(product6); 
      
      if (!subtract_check) { total = total + product6; tx_str = tx_str + P_6 + "    :   " + product6 + "\n";} 
      if (subtract_check) { total = total - product6; tx_str = tx_str + P_6 + "    :  -" + product6 + "\n"; subtract_check=0; } }
    
    if (card7[0] == uid[0] && card7[1] == uid[1] && card7[2] == uid[2] && card7[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 7"); lcd.setCursor(0,3); lcd.print(product7); 
      
      if (!subtract_check) { total = total + product7; tx_str = tx_str + P_7 + "    :   " + product7 + "\n";} 
      if (subtract_check) { total = total - product7; tx_str = tx_str + P_7 + "    :  -" + product7 + "\n"; subtract_check=0; } }
    
    if (card8[0] == uid[0] && card8[1] == uid[1] && card8[2] == uid[2] && card8[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 8"); lcd.setCursor(0,3); lcd.print(product8); 
      
      if (!subtract_check) { total = total + product8; tx_str = tx_str + P_8 + "    :   " + product8 + "\n";} 
      if (subtract_check) { total = total - product8; tx_str = tx_str + P_8 + "    :  -" + product8 + "\n"; subtract_check=0; } }
   
    if (card9[0] == uid[0] && card9[1] == uid[1] && card9[2] == uid[2] && card9[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 9"); lcd.setCursor(0,3); lcd.print(product9); 
      
      if (!subtract_check) { total = total + product9; tx_str = tx_str + P_9 + "    :   " + product9 + "\n";} 
      if (subtract_check) { total = total - product9; tx_str = tx_str + P_9 + "    :  -" + product9 + "\n"; subtract_check=0; } }  
    
    if (card10[0] == uid[0] && card10[1] == uid[1] && card10[2] == uid[2] && card10[3] == uid[3]) { 
      
      lcd.setCursor(7,2); lcd.print("Card 10"); lcd.setCursor(0,3); lcd.print(product10); 
      
      if (!subtract_check) { total = total + product10; tx_str = tx_str + P_10 + "   :   " + product10 + "\n";} 
      if (subtract_check) { total = total - product10; tx_str = tx_str + P_10 + "   :  -" + product10 + "\n"; subtract_check=0; } }          

    lcd.setCursor(5,3); lcd.print("Total : "); lcd.print(total); lcd.print(" Rs");
        
    Serial.println("");
    success = 0;
    // Wait 1 second before continuing
    delay(1000);
  }
  //else
  //{
    // PN532 probably timed out waiting for a card
    //Serial.println("Timed out waiting for a card");
  //}                   // Wait for two seconds (to demonstrate the active low LED)

}
