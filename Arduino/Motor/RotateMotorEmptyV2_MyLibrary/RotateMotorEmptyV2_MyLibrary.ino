#include "Motor.h"
#include <PCF8574.h>
using namespace System;

//setting adresses for motor extender
PCF8574 pcf8574(0x21);
const int DirXPin = P0;
const int StepXPin = P1;

const int DirYPin = P2;
const int StepYPin = P3;

const int DirZPin = P4;
const int StepZPin = P5;

const int DirAPin = P6;
const int StepAPin = P7;

//setting adresses for buttons extender
PCF8574 pcf8574_Button(0x20);
const int  EnPin = P6;
const int Button1 = P0;
const int Button2 = P1;
const int Button3 = P2;
const int Button4 = P3;
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

void setup() {
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

    //Print on screen to check if sending data to pcf8574
     if (pcf8574.begin())
    {
        Serial.println(":\n Sending data to PCF8574.");
    }
    else
    {
        Serial.println("Error while sending data to PCF8574.");
    }

    m1.Rotate(false, 200);

}

void loop() {
   m1.Process();

}
