// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace System
{
	class Timer;
	typedef void (*TimerEvent)(Timer*);

	class Timer
	{
	public:
		TimerEvent EventElapsed;
		void* EventObject;
		unsigned long interval, startTime;


		Timer();
		Timer(unsigned long interval);

		void Start();
		void Stop();
		void SetInterval(unsigned long value);
		void Process();
	};

}


#endif

