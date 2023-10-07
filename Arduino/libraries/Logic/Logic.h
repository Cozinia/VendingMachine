// Logic.h

#ifndef _LOGICS_h
#define _LOGICS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#define COIN_DEBOUNCING_TIME 30
#define COIN_WAIT_INTERVAL 25
#define COIN_WAIT_INTERVAL1 40
#define TIMER 200
typedef int (*Pulses)();
namespace System {
	class Coin;
	typedef void (*CoinEvent)(Coin*);
	class Coin
	{
	protected:
		int pulses;
		int p1, p2, p3;
		unsigned long pulsesTime;
		unsigned long readTime;
		int pulsesCounter;
		Pulses puls;

		int crtValue, prevValue;
		int count;
		float credit;
		unsigned long crtTime, prevTime;
		int mytime;
		bool changed;
		int number;
		unsigned long  Time1, Time2, Time3;
		int totalCoins1, totalCoins2, totalCoins3;
		int crtSec;
		int event;


	public:
		float totalCredit;
		Coin();
		Coin(Pulses numberOfPulses);
		float CheckPulses(int pulses);
		void Process();

	public:
		CoinEvent PulsEvent;


	};



}

#endif
