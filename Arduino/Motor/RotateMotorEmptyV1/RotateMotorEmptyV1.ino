#include <PCF8574.h>

PCF8574 pcf8574(0x21);
const int DirXPin = P0;
const int StepXPin = P1;

const int DirYPin = P2;
const int StepYPin = P3;

const int DirZPin = P4;
const int StepZPin = P5;
const int  EnPin = P6;




unsigned int steps = 16;
unsigned int steps1 = 8;
unsigned int steps2 = 4;
unsigned int steps3 = 2;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    pcf8574.pinMode(StepXPin, OUTPUT);
    pcf8574.pinMode(DirXPin, OUTPUT);
    pcf8574.pinMode(StepYPin, OUTPUT);
    pcf8574.pinMode(DirYPin, OUTPUT);
    pcf8574.pinMode(StepZPin, OUTPUT);
    pcf8574.pinMode(DirZPin, OUTPUT);
    pcf8574.pinMode(EnPin, OUTPUT);;
    if (pcf8574.begin())
    {
        Serial.println("Sending data to PCf8574.");
    }
    else
    {
        Serial.println("Error while sending data to PCf8574.");
    }
/*
    //Rotate motor
     pcf8574.digitalWrite(EnPin, LOW);
        pcf8574.digitalWrite(DirPin, LOW);
        Serial.println("Rotate() entered");
        for (int MotorDir = 0; MotorDir < 200; MotorDir++)
        { // loop for 200 steps
            pcf8574.digitalWrite(StepPin, HIGH);
            delayMicroseconds(1000);
            pcf8574.digitalWrite(StepPin, LOW);
            delayMicroseconds(1000);
        }
        pcf8574.digitalWrite(EnPin, HIGH);
        */
}

int i = 0;
void loop()
{
  //nothing to do here
  if(i==0)
  {
   pcf8574.digitalWrite(EnPin, LOW);
        pcf8574.digitalWrite(DirZPin, HIGH);
        Serial.println("Rotate() entered");
         delay(100);
  }
        if(i < steps2*200)
        { // loop for 200 steps
            pcf8574.digitalWrite(StepZPin, HIGH);
            delayMicroseconds(300);//500
            pcf8574.digitalWrite(StepZPin, LOW);
            delayMicroseconds(300);//500
             Serial.printf("\r\nsteps %d ",i );
             i++;
        }
    if(i==steps2*200)
        {
         delay(100);
          pcf8574.digitalWrite(EnPin, HIGH);
          i++;
        }



}
