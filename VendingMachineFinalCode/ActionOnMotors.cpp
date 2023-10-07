// 
// 
// 

#include "ActionOnMotors.h"
using namespace System;


PCF8574 pcf8574(0x21);
const int DirXPin = P0;
const int StepXPin = P1;

const int DirYPin = P2;
const int StepYPin = P3;

const int DirZPin = P4;
const int StepZPin = P5;

const int DirAPin = P6;
const int StepAPin = P7;

PCF8574 pcf8574_Button(0x20);
const int  EnPin = P6;
const int Button1 = P0;
const int Button2 = P1;
const int Button3 = P2;
const int Button4 = P3;
const int RelayPin = P5;
const int FixPin = P7;
const int UserLogOutButton = P4;
extern float credit;

LiquidCrystal_I2C lcd(0x27, 16, 2);
char MyBuffer[BufferSize];
char TempBuffer[BufferSize];


void SetPcfPin(int pin, int value)
{

    if (pin == EnPin)
    {
        pcf8574_Button.digitalWrite(pin, value);
    }
    else
    {
        pcf8574.digitalWrite(pin, value);
    }

}

Motor m1(EnPin, DirXPin, StepXPin, SetPcfPin);
Motor m2(EnPin, DirYPin, StepYPin, SetPcfPin);
Motor m3(EnPin, DirZPin, StepZPin, SetPcfPin);
Motor m4(EnPin, DirAPin, StepAPin, SetPcfPin);


int Button1_Get()
{
	int pinValue = !pcf8574_Button.digitalRead(Button1);
	//Serial.println(pinValue);
	return pinValue;
}
int Button2_Get()
{
	int pinValue = !pcf8574_Button.digitalRead(Button2);
	//Serial.println(pinValue);
	return pinValue;
}
int Button3_Get()
{
	int pinValue = !pcf8574_Button.digitalRead(Button3);
	//Serial.println(pinValue);
	return pinValue;
}
int Button4_Get()
{
	int pinValue = !pcf8574_Button.digitalRead(Button4);
	//Serial.println(pinValue);
	return pinValue;
}
int UserLogOutButton_Get()
{
	int pinValue = !pcf8574_Button.digitalRead(UserLogOutButton);
	//Serial.println(pinValue);
	return pinValue;
}

void Button1_product();
void Button2_product();
void Button3_product();
void Button4_product();

Button b1(Button1_Get);
Button b2(Button2_Get);
Button b3(Button3_Get);
Button b4(Button4_Get);
Button b5(UserLogOutButton_Get);

void Button1_Down(Button* sender)
{
	Serial.println("Down");

	Button1_product();


}
void Button2_Down(Button* sender)
{
	Serial.println("Down");
	Button2_product();

}
void Button3_Down(Button* sender)
{
	Serial.println("Down");
	Button3_product();

}
void Button4_Down(Button* sender)
{
	Serial.println("Down");
	Button4_product();

}
void Button5_Down(Button* sender)
{
	Serial.println("UserLogOff");
	credit = 0;

}

void ActionOnMotors_Init() {
    pcf8574.pinMode(StepXPin, OUTPUT);
    pcf8574.pinMode(DirXPin, OUTPUT);
    pcf8574.pinMode(StepYPin, OUTPUT);
    pcf8574.pinMode(DirYPin, OUTPUT);
    pcf8574.pinMode(StepZPin, OUTPUT);
    pcf8574.pinMode(DirZPin, OUTPUT);
    pcf8574.pinMode(StepAPin, OUTPUT);
    pcf8574.pinMode(DirAPin, OUTPUT);

    pcf8574_Button.pinMode(EnPin, OUTPUT);
    pcf8574_Button.pinMode(Button1, INPUT_PULLUP);
    pcf8574_Button.pinMode(Button2, INPUT_PULLUP);
    pcf8574_Button.pinMode(Button3, INPUT_PULLUP);
    pcf8574_Button.pinMode(Button4, INPUT_PULLUP);
	pcf8574_Button.pinMode(UserLogOutButton, INPUT_PULLUP);
	pcf8574_Button.pinMode(RelayPin, OUTPUT);
	pcf8574_Button.pinMode(FixPin, OUTPUT);
	pcf8574_Button.digitalWrite(RelayPin, LOW);
	pcf8574_Button.digitalWrite(FixPin, HIGH);

    lcd.init();
    lcd.backlight();
    lcd.setBacklight(HIGH);

	b1.EventDown = Button1_Down;
	b2.EventDown = Button2_Down;
	b3.EventDown = Button3_Down;
	b4.EventDown = Button4_Down;
	b5.EventDown = Button5_Down;

    //Print on screen to check if sending data to pcf8574
    if (pcf8574.begin())
    {
        Serial.println(":\n Sending data to PCF8574.");
    }
    else
    {
        Serial.println("Error while sending data to PCF8574.");
    }

    //Print on screen to check if sending data to pcf8574
    if (pcf8574_Button.begin())
    {
        Serial.println(":\n Sending data to PCF8574.");
    }
    else
    {
        Serial.println("Error while sending data to PCF8574.");
    }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please wait");

   // m1.Rotate(true, 400);
}

void Motor1()
{
    Serial.print("\nRotate motor m1");
    m1.Rotate(true, 400);
    lcd.clear();

    for (int i = 100; i >= 0; i--)
    {
        lcd.setCursor(0, 0);
        lcd.print("Test motor 1");
    }
    lcd.clear();
    server.send(200, "text/plain", "Rotate motor 1");
   
}

void Motor2()
{
    Serial.print("\nRotate motor m2");
    m2.Rotate(true, 400);
    lcd.clear();

    for (int i = 100; i >= 0; i--)
    {
        lcd.setCursor(0, 0);
        lcd.print("Test motor 2");
    }
    lcd.clear();
    server.send(200, "text/plain", "Rotate motor 2");
}

void Motor3()
{
    Serial.print("\nRotate motor m3");
    m3.Rotate(true, 400);
    lcd.clear();

    for (int i = 100; i >= 0; i--)
    {
        lcd.setCursor(0, 0);
        lcd.print("Test motor 3");
    }
    lcd.clear();
    server.send(200, "text/plain", "Rotate motor 3");
}

void Motor4()
{
    Serial.print("\nRotate motor m4");
    m4.Rotate(false, 400);
    lcd.clear();

    for (int i = 100; i >= 0; i--)
    {
        lcd.setCursor(0, 0);
        lcd.print("Test motor 4");
    }
    lcd.clear();
    server.send(200, "text/plain", "Rotate motor 4");
}

void OpenDoor() {
	unsigned long Time = millis();
	
	while (millis() - Time <= 1000) {
		pcf8574_Button.digitalWrite(RelayPin, LOW);
		pcf8574_Button.digitalWrite(FixPin, LOW);
		
	}
	pcf8574_Button.digitalWrite(RelayPin, HIGH);
	delay(100);
	pcf8574_Button.digitalWrite(FixPin, LOW);
	server.send(200);
}

void Button_Up(Button* sender)
{
	Serial.println("Up");
}


void Button1_product()
{

	if (credit >= 1.00)
	{
		credit = credit - 1.00;

		fileDb.products[0].Quantity = fileDb.products[0].Quantity - 1;

		m1.Rotate(true, 400);
		lcd.clear();
		for (int i = 30; i >= 0; i--)
		{
			lcd.setCursor(1, 0);
			lcd.print("item selected:");
			lcd.setCursor(3, 1);
			lcd.print("Snikers");
		}
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Credit:");
		lcd.setCursor(7, 0);
		lcd.print(credit);
	}
	else
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Not enough ");
		lcd.setCursor(0, 1);
		lcd.print("credit");
	}

}

void Button2_product()
{
	if (credit >= 1.50)
	{
		credit = credit - 1.50;
		fileDb.products[1].Quantity = fileDb.products[0].Quantity - 1;
		m2.Rotate(true, 400);
		lcd.clear();
		for (int i = 30; i >= 0; i--)
		{
			lcd.setCursor(1, 0);
			lcd.print("item selected:");
			lcd.setCursor(3, 1);
			lcd.print("Mars");
		}
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Credit:");
		lcd.setCursor(7, 0);
		lcd.print(credit);

	}
	else
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Not enough ");
		lcd.setCursor(0, 1);
		lcd.print("credit");
	}
}

void Button3_product()
{
	if (credit >= 2.00)
	{
		credit = credit - 2.00;
		fileDb.products[2].Quantity = fileDb.products[2].Quantity - 1;
		m3.Rotate(true, 400);
		lcd.clear();
		for (int i = 30; i >= 0; i--)
		{
			lcd.setCursor(1, 0);
			lcd.print("item selected:");
			lcd.setCursor(3, 1);
			lcd.print("Twix");
		}
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Credit:");
		lcd.setCursor(7, 0);
		lcd.print(credit);

	}
	else
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Not enough ");
		lcd.setCursor(0, 1);
		lcd.print("credit");
	}
}

void Button4_product()
{
	if (credit >= 1.50)
	{
		credit = credit - 1.50;
		fileDb.products[3].Quantity = fileDb.products[3].Quantity - 1;
		m4.Rotate(true, 400);
		lcd.clear();
		for (int i = 30; i >= 0; i--)
		{
			lcd.setCursor(1, 0);
			lcd.print("item selected:");
			lcd.setCursor(3, 1);
			lcd.print("7Days");
		}
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Credit:");
		lcd.setCursor(7, 0);
		lcd.print(credit);

	}
	else
	{
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Not enough ");
		lcd.setCursor(0, 1);
		lcd.print("credit");
	}
}

void ActionOnMotors_Process() {

	b1.Process();
	m1.Process();
	b2.Process();
	m2.Process();
	b3.Process();
	m3.Process();
	b4.Process();
	m4.Process();
	b5.Process();
}
