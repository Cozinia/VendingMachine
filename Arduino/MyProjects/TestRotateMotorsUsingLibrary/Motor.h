// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


#define MOTOR_WAIT_INTERVAL 10
typedef void (*SetPinFunc)(int pin, int value);


namespace System
{

  class Motor
  {
  protected:
    int enPin;
    int dirPin;
    int stepPin;
    SetPinFunc setPinFunc;

    int steps;


    unsigned long rotateTime;



  public:
    Motor(int enPin, int dirPin, int stepPin, SetPinFunc setPin);
    void Rotate(bool dir, int steps);
    void Cancel();
    void Process();


  protected:


  };
}
#endif
