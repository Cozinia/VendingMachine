// RFID.h

#ifndef _RFID_h
#define _RFID_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <SPI.h>
#include <MFRC522.h>
#include "FileDb.h"
#include "ActionOnMotors.h"
#define RST_PIN D4  
#define SS_PIN D8

extern MFRC522 mfrc522;

void RFID_Init();
void RFID_Process();
#endif

