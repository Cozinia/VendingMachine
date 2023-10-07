// ActionOnMotors.h

#ifndef _ACTIONONMOTORS_h
#define _ACTIONONMOTORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <PCF8574.h>
#include "WebServer.h"
#include <LiquidCrystal_I2C.h>
#include <Motor.h>
#include <Button.h>
#include "CoinAcceptor.h"
#include "FileDb.h"

extern LiquidCrystal_I2C lcd;

//setting adresses for motor extender
extern PCF8574 pcf8574;
extern	const int DirXPin;
extern	const int StepXPin;

extern	const int DirYPin;
extern	const int StepYPin;

extern	const int DirZPin;
extern	const int StepZPin;

extern	const int DirAPin;
extern	const int StepAPin;
//setting adresses for buttons extender
extern	PCF8574 pcf8574_Button;
extern	const int  EnPin;
extern	const int Button1;
extern	const int Button2;
extern	const int Button3;
extern	const int Button4;
extern const int RelayPin;
extern const int FixPin;
extern const int UserLogOutButton;
void ActionOnMotors_Init();
void ActionOnMotors_Process();
void SetPcfPin(int pin, int value);

void Motor1();
void Motor2();
void Motor3();
void Motor4();
void OpenDoor();

#endif

