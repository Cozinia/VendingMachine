// 
// 
// 

#include "RFID.h"
extern LiquidCrystal_I2C lcd;
MFRC522 mfrc522(SS_PIN, RST_PIN);
String tag;
String prevTag = " ";
void RFID_Init() {
	SPI.begin();      // Init SPI bus
	mfrc522.PCD_Init();   // Init MFRC522
}

void RFID_CheckTag()
{
    unsigned long StartTime = millis();
    for (int i = 0; i <= fileDb.userCount; i++)
    {
        if (strcmp(fileDb.users[i].TagNumber, tag.c_str()) == 0)
        {
            credit = fileDb.users[i].Credit;
            lcd.clear();
          /*  while (millis() - StartTime < 1000)*/
            {
                lcd.setCursor(0, 0);
                lcd.printf("Hello, %s!", fileDb.users[i].FirstName);
                lcd.setCursor(0, 1);
                lcd.printf("Credit %f", fileDb.users[i].Credit);
            }

        }
    }
}
   




void RFID_Process()
{

        if (!mfrc522.PICC_IsNewCardPresent())
            return;
        if (mfrc522.PICC_ReadCardSerial())
        {
            for (byte i = 0; i < 4; i++)
            {
                tag += mfrc522.uid.uidByte[i];
            }
            Serial.println(tag);
        }
        RFID_CheckTag();
        tag = "";
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

}

