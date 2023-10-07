//Luat de aici: http://www.esp8266learning.com/esp8266-rfid-rc522-module-example.php

#include <SPI.h>
#include <MFRC522.h>
 
#define RST_PIN         D3         // Configurable, see typical pin layout above
#define SS_PIN          D8        // Configurable, see typical pin layout above
 
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
 String tag;
void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
 
void loop() {
  /*
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
 
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
 
  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  */

    if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  if (mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += mfrc522.uid.uidByte[i];
    }
    Serial.println(tag);
    if(tag == "22918314235"){
      Serial.println("User 1");
      }
    if(tag == "167116175"){
      Serial.println("User 2");
      }
    if(tag == "398773166"){
      Serial.println("User 3");
      }
    tag = "";
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}
