// 
// 
// 

#include "Timer.h"

namespace System
{



	Timer::Timer()
	{
		EventElapsed = NULL;
		interval = 0;
		startTime = 1;
		EventObject = NULL;
	}

	Timer::Timer(unsigned long interval) :Timer()
	{
		this->interval = interval;
	}

	void Timer::Start()
	{
		startTime = millis();
	}
	void Timer::Stop()
	{
		startTime = 0;
	}
	void Timer::SetInterval(unsigned long value)
	{
		interval = value;
	}

	void Timer::Process()
	{
		if (startTime > 0)
		{
			if (millis() - startTime >= interval)
			{
				startTime = 0;
				if (EventElapsed)
					EventElapsed(this);
				Serial.print("Passing time...");
			}
		}
	}

}
