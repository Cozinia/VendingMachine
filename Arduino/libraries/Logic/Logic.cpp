#include "Logic.h"

namespace System {

    Coin::Coin()
    {
        pulses = 0;
        p1 = 1;
        p2 = 2;
        p3 = 3;
        puls = NULL;
        readTime = 0;
        pulsesTime = 0;
        pulsesCounter = 0;
        totalCredit = 0;

		crtValue = 0;
		prevValue = 0;
		count = 0;
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

    }

    Coin::Coin(Pulses numberOfPulses)
    {
        puls = numberOfPulses;
    }

    float Coin::CheckPulses(int pulses)
    {
        if (pulses)
        {
            number++;
        }
        if (number == 1)
        {
            totalCredit = totalCredit + 0.05;
            number = 0;
        }
        if (number == 2)
        {
            totalCredit = totalCredit + 0.10;
            number = 0;
        }
        if (number = 3)
        {
            totalCredit = totalCredit + 0.50;
            number = 0;
        }
        Serial.print("Credit: ");
        Serial.print(totalCredit);
        Serial.print("\n");
    }


    void Coin::Process()
    {
		crtTime = millis();

		if (crtTime > prevTime)
		{

			if (crtValue != prevValue)
			{
				prevTime = crtTime;

				prevValue = crtValue;
				changed = true;
			}

			if (crtTime - prevTime > 40 && changed)
			{
				count++;
				Serial.print(count);
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
			else
				if (number == 2)
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

						Serial.print("Credit: ");
						Serial.print(credit);
						Serial.print("\n");
					}
		}

    }

}
