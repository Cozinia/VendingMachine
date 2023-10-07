#include "Motor.h"


namespace System
{

	Motor::Motor(int enPin, int dirPin, int stepPin, SetPinFunc setPin)
	{
		this->enPin = enPin;
		this->dirPin = dirPin;
		this->stepPin = stepPin;
		this->setPinFunc = setPin;

		steps = 0;

		rotateTime = 0;
	}

	void Motor::Rotate(bool dir, int steps)
	{
		Serial.println("Rotate");
		this->steps = steps * 2;
		setPinFunc(enPin, LOW);
		setPinFunc(dirPin, dir);

	}

	void Motor::Cancel()
	{
		steps = 0;
		setPinFunc(enPin, HIGH);
		Serial.println("Cancel() entered");

	}

	void Motor::Process()
	{
		if (steps)
		{

			//Serial.println("steps");
			if (millis() - rotateTime >= MOTOR_WAIT_INTERVAL)
			{

				if ((steps % 2) == 0)
				{
					setPinFunc(stepPin, HIGH);
					//Serial.println(steps);

				}
				else
				{
					setPinFunc(stepPin, LOW);
				}
				rotateTime = millis();
				steps--;

				if (!steps)
					Cancel();
			}

		}




	}


}