#include "Wire.h"
#include "PN532_I2C.h"
#include "PN532.h"

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

String userRFID = "";

void setup(void)
{

  Serial.begin(9600);
  Serial.println("NFC/RFID Reader");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  //  Non-blocking procedure
  nfc.setPassiveActivationRetries(0x01);
 
  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
}


void loop()
{
  readRFID();
}

void readRFID(void)
{
  boolean success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success)
  {
    for (uint8_t i = 0; i < uidLength; i++)
    {
      if (uid[i] <= 0xF) {
        userRFID += "0";
      }
      userRFID += String(uid[i] & 0xFF, HEX);
    }
    userRFID.toUpperCase();
    Serial.println(userRFID);
    delay(400);
    userRFID = "";
  }
}
