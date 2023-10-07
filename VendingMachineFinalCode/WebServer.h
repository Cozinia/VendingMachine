// WebServer.h

#ifndef _WEBSERVER_h
#define _WEBSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <FS.h>
#include <PCF8574.h>
#include "ActionOnMotors.h"
#define BufferSize 500


extern ESP8266WebServer server;


void WebServer__GetIndexPage();
void WebServer__Init();
void WebServer__Process();


extern char MyBuffer[BufferSize];
extern char TempBuffer[BufferSize];
#endif



