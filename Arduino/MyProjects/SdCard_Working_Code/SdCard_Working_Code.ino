#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>

void htmlIndex();

//------------------Internet connection------------------------
char* ssid = "Telekom-00E3";
char* password = "6DKY7ELEXZ9SZ";
ESP8266WebServer server(80);
//-----------------------------------------Web page------------------------------------------
String storage = "";

//the page variable is used to store the message sent to the
//user

//below you can find the html page
//more details about the html page can be found in the guide

//--------------------------Starting the web server-----------------------------

//connectToWiFi() is used to connect to the WiFi network
//using the SSID and PASSWORD variables created at the
//beginning of the code
void connectToWiFi()
{
  Serial.println("Connecting to the WiFi");
  //set the WiFi mode to WiFi_STA.
  //the WiFi_STA means that the board will act as a station,
  //similar to a smartphone or laptop
  WiFi.mode(WIFI_STA);

  //connect to the WiFi network using the ssid and password strings
  WiFi.begin(ssid, password);

  //below we check the status of the WiFi and wait until the
  //board is connected to the network
  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //when the board is successfully connected to the network,
  //display the IP assigned to it in the serial monitor.
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//setupServer() function is used to set up and organise
//the website
void setupServer()
{
      SPI.begin();               // Init SPI bus
  while (!SD.begin(15)) {
    delay(200);
  }



  //the method "server.on()" is to call a function when
  //the user access the location
  //the default location is "/"
  server.on("/", htmlIndex);

  //start the server
  server.begin();

  //print in Serial Monitor that the HTTP server is started
  Serial.println("HTTP server started");
}
//--------------------------------------Setup----------------------------------------
void setup()
{
  
  Serial.begin(9600);
  connectToWiFi();
  setupServer();

}
//---------------------------------Loop-----------------------------
void loop()
{
  server.handleClient();
}

//----------------------Access the files from SD card---------------------------
void htmlIndex()
{
  File file = SD.open("index.htm");
if(file){
size_t sent = server.streamFile(file,"text/html");
file.close();
}
}
