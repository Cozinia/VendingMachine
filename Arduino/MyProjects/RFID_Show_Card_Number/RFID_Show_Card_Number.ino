/* Read RFID Tag with RC522 RFID Reader
 *  Made by miliohm.com
 *  Luat de aici: https://miliohm.com/rc522-rfid-reader-with-nodemcu/
 */
 
#include <SPI.h>
#include <MFRC522.h>

const int RST_PIN = D3;     // Configurable, see typical pin layout above
const int SS_PIN = D8;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
