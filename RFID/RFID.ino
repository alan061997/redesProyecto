/*
Many thanks to nikxha from the ESP8266 forum
*/
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = GPIO5 (D1)
SDA(SS) = GPIO4 (D2)
MOSI    = GPIO13 (D7)
MISO    = GPIO12 (D6)
SCK     = GPIO14 (D5)
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN  5  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  4  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

const char *ssid =  "Moto E (4) 6565";     // Cambiar de acuerdo a la RED
const char *pass =  "12345678"; // Cambiar de acuerdo a la RED
const char *hostGet = "192.168.43.73/test.php";
String id_string[] = {
  "D5 A5 E8 AB ",
  "D0 7A 81 4D "
};


MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance



void setup() {
  Serial.begin(115200);    // Initialize serial communications
  delay(250);
  Serial.println(F("Booting...."));
  
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  
  WiFi.begin(ssid, pass);
  
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected"));
  }
  
  Serial.println(F("Ready!"));
  Serial.println(F("======================================================")); 
  Serial.println(F("Scan for Card and print UID:"));
}

void loop() { 
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
 
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  String UID = "";
  for (byte i = 0; i < bufferSize; i++) {
    buffer[i] < 0x10 ? " 0" : " ";
    UID = UID + (String(buffer[i], HEX))+" ";
  }
  UID.toUpperCase();

  String ID_ULT_NUM = "1";
  
  for (int i = 0; i < 3; i++){
    if (UID == id_string[i]){
      ID_ULT_NUM = String(i+1);
      break;  
    }
  }
  
  Serial.println(UID);

  if(WiFi.status()== WL_CONNECTED){                                         //Check WiFi connection status
  
     HTTPClient http;                                                       //Declare object of class HTTPClient
  
     http.begin("http://192.168.43.73/test.php");                            //Specify request destination
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");   //Specify content-type header
     String request = "TagID="+ID_ULT_NUM;
     int httpCode = http.POST(request);                                     //Send the request
     String payload = http.getString();                                     //Get the response payload
  
     Serial.println(httpCode);                                              //Print HTTP return code
     Serial.println(payload);                                               //Print request response payload
  
     http.end();                                                            //Close connection
  
   }else{
  
      Serial.println("Error in WiFi connection");   
  
   }
  
    delay(3000);                                                            //Send a request every 30 seconds
  

}
