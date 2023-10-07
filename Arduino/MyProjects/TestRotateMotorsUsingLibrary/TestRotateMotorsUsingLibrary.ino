#include "Motor.h"
#include <PCF8574.h>
#include "Button.h"

using namespace System;

//setting adresses for motor extender
PCF8574 pcf8574(0x21);
const int  EnPin = P0;
const int DirXPin = P1;
const int StepXPin = P2;
const int Button1 = P3;

void SetPcfPin(int pin, int value)
{
  if (pin == EnPin)
  {
    pcf8574.digitalWrite(pin, value);
  }
  else
  {
    pcf8574.digitalWrite(pin, value);
  }
}

void Button_Up(Button* sender)
{
  Serial.println("Up");
}

int Button1_Get()
{
  int pinValue = !pcf8574.digitalRead(Button1);
  return pinValue;
}

void Button1_product();

Button b1(Button1_Get);

Motor m1(EnPin, DirXPin, StepXPin, SetPcfPin);

void Button1_Down(Button* sender)
{
  Serial.println("Down");    
  Button1_product(); 
}

void setup()
{
  //----------------Declarations---------------------
  Serial.begin(9600);

  pcf8574.pinMode(StepXPin, OUTPUT);
  pcf8574.pinMode(DirXPin, OUTPUT);

  pcf8574.pinMode(EnPin, OUTPUT);
  pcf8574.pinMode(Button1, INPUT_PULLUP);

  if (pcf8574.begin())
  {
    Serial.println(":\n Sending data to PCF8574.");
  }
  else
  {
    Serial.println("Error while sending data to PCF8574.");
  }
  b1.EventUp = Button_Up;
  b1.EventDown = Button1_Down;

}

void loop()
{
  b1.Process();
  m1.Process();

}

void Button1_product()
{
    m1.Rotate(false, 200);
  }
