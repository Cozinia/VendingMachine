// 
// 
// 
#include "Arduino.h"
#include "Button.h"

namespace System
{

    Button::Button()
    {
        pressed = 0;
        stateChangeTime = 0;
        EventDown = 0;
        EventUp = 0;
        readTime = 0;
        getValueFunc = NULL;

    }

    Button::Button(GetPinFunc func) : Button()
    {
        getValueFunc = func;
    }

    void Button::setPressed(bool value)
    {
        if (pressed != value)
        {
            pressed = value;
            if (pressed)
            {

                if (EventDown)
                {
                    EventDown(this);
                }
            }
            else
            {
                if (EventUp)
                {
                    EventUp(this);
                }
            }
        }
    }

    void Button::Process()
    {
        if (!getValueFunc)
            return;

        if (millis() - readTime > BUTTON_WAIT_INTERVAL)
        {
            int pinState = 0;
            pinState = getValueFunc();
            readTime = millis();
            //if(pinState)
            //    Serial.println(pinState);

            if (!stateChangeTime)
            {

                if (pinState != pressed)
                {
                    //Serial.print("pinState = !pressed\r\n");
                    stateChangeTime = millis();
                }
            }
            else
            {
                if (millis() - stateChangeTime > BUTTON_DEBOUNCING_TIME)
                {
                    //Serial.print("millis() - stateChangeTime > BUTTON_DEBOUNCING_TIME\r\n");
                    stateChangeTime = 0;
                    setPressed(pinState);
                }
            }
        }
        //Serial.print(pinState);


    }
}
