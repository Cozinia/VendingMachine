// CoinAcceptor.h

#ifndef _COINACCEPTOR_h
#define _COINACCEPTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#endif
#include "ActionOnMotors.h"
extern float credit;
void CoinAcceptor_Init();
void CoinAcceptor_Process();