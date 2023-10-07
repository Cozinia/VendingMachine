/*
 Name:		VendingMachineFinalCode.ino
 Created:	5/17/2022 1:21:05 PM
 Author:	Cozinia
*/

// the setup function runs once when you press reset or power the board


#include "RFID.h"
#include "CoinAcceptor.h"
#include "ActionOnMotors.h"
#include <LiquidCrystal_I2C.h>
#include "Functions.h"
#include <SPI.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "WebServer.h"
#include "FileDb.h"
#include "ActionOnMotors.h"
#include "CoinAcceptor.h"
const char* const ssid = "iPhone 11";
const char* const password = "aeronautic";



void InitWiFi()
{
	Serial.println("\nConnecting to the WiFi");
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println("Waiting for connection");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}


void setup() {
	Serial.begin(115200);
	SPIFFS.begin();
	unsigned long StartTime = millis();
	InitWiFi();
	WebServer__Init();
	fileDb.init();
	fileDb.InitData();
	ActionOnMotors_Init();
	RFID_Init();
	lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Please wait...");
		pcf8574_Button.digitalWrite(FixPin, LOW);
		//pcf8574_Button.digitalWrite(FixPin, 0);
	OpenDoor();
	CoinAcceptor_Init();
	lcd.clear();
}

// the loop function runs over and over again until power down or reset
void loop() {
	WebServer__Process();
	ActionOnMotors_Process();
	CoinAcceptor_Process();
	RFID_Process();
	server.handleClient();
	
}


