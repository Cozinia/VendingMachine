// 
// 
// 

#include "CoinAcceptor.h"

extern LiquidCrystal_I2C lcd;
const int CountPin = D3;
#define TIMER 200
//--Variables------
int crtValue, prevValue;
int counter;
float credit;
int product;
unsigned long crtTime, prevTime;
int mytime;
bool changed;
int number;
unsigned long  Time1, Time2, Time3;
int totalCoins1, totalCoins2, totalCoins3;
int crtSec;
int event;

void CoinEvent()
{
    crtValue = digitalRead(CountPin);

    Serial.print("CoinEVENT_____________");
    Serial.print("\n");
    Serial.print(crtValue);
    Serial.print("\n");

}

void CoinAcceptor_Init() {
	pinMode(CountPin, INPUT);
	lcd.init();
	lcd.backlight();
	lcd.setBacklight(HIGH);
	crtValue = 0;
	prevValue = 0;
	counter = 0;
	crtTime = 0;
	prevTime = 0;
	crtSec = 0;
	changed = false;
	credit = 0;
	number = 0;
	Time1 = 0;
	Time2 = 0;
	Time3 = 0;
	totalCoins1 = 0;
	totalCoins2 = 0;
	totalCoins3 = 0;
	lcd.setCursor(0, 0);
	lcd.print("Welcome! ;)");
	Serial.print("Welcome! ;)");

	lcd.setCursor(0, 0);
	lcd.print("Please insert");
	lcd.setCursor(0, 1);
	lcd.print(" coins");

	lcd.setCursor(0, 0);
	lcd.print("Or press -> btn");
	lcd.setCursor(0, 1);
	lcd.print("or <- button");
    attachInterrupt(digitalPinToInterrupt(CountPin), CoinEvent, CHANGE);
}

void totalCoins()
{
    Time3 = millis();
    if (Time3 - Time1 > 8000)
    {
        //show on serial monitor the number of coins for each type
        //0.50 coins
        Serial.print("Total 0.50 coin inserted: ");
        Serial.print(totalCoins3);
        Serial.print("\n");
        //0.10 coins
        Serial.print("Total 0.10 coin inserted: ");
        Serial.print(totalCoins2);
        Serial.print("\n");
        //0.05 coins
        Serial.print("Total 0.05 coin inserted: ");
        Serial.print(totalCoins1);
        Serial.print("\n");
    }
}

void Coins()
{
    //take current time
    crtTime = millis();
    //compare current time with previous time
    if (crtTime > prevTime)
    {
        //compare current value with previous value
        if (crtValue != prevValue)
        {
            prevTime = crtTime;

            prevValue = crtValue;
            changed = true;
        }
        if (crtTime - prevTime > 30 && changed)
        {
            counter++;
            Serial.print(counter);
            Serial.print("\n");
            changed = false;
            number++;
            Time1 = millis();
            changed = false;
        }
    }
    Time2 = millis();
    if ((Time2 - Time1 > TIMER) && (number))
    {

        Serial.print("Number: ");
        Serial.print(number);
        Serial.print("\n");
        Serial.print("Time2-Time1: ");
        Serial.print(Time2 - Time1);
        Serial.print("\n");

        if (number == 3)
        {
            credit = credit + 0.50;
            totalCoins3++;
            number = 0;
        }
        else if (number == 2)
        {
            credit = credit + 0.10;
            totalCoins2++;
            number = 0;
        }
        else
            if (number == 1)
            {
                totalCoins1++;
                credit = credit + 0.05;
                number = 0;
            }
        lcd.setCursor(0, 0);
        lcd.print("Credit: ");
        lcd.setCursor(8, 0);
        lcd.print(credit);
        Serial.print("Credit: ");
        Serial.print(credit);
        Serial.print("\n");
    }
}

void CoinAcceptor_Process() {
	Coins();
}