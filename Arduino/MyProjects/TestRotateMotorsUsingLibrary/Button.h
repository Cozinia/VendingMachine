// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define BUTTON_DEBOUNCING_TIME 30
#define BUTTON_SHORT_PRESS_TIME 300

#define BUTTON_WAIT_INTERVAL 10
typedef int (*GetPinFunc)();

namespace System
{

  class Button;
  typedef void (*ButtonEvent)(Button*);

  class Button
  {

  protected:
    //int pin;
    bool pressed;
    unsigned long stateChangeTime;
    unsigned long readTime;
    GetPinFunc getValueFunc;

  public:
    ButtonEvent EventDown, EventUp;



  public:
    Button();
    Button(GetPinFunc func);

    void Process();

  protected:
    void setPressed(bool value);

  };


}



#endif
