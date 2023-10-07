/*------------------------------------------------------------------------------
  07/01/2018
  Author: Makerbro
  Platforms: ESP8266
  Language: C++/Arduino
  File: webserver_html_js.ino
  ------------------------------------------------------------------------------
  Description: 
  Code for YouTube video demonstrating how to use JavaScript in HTML weppages
  that are served in a web server's response.
  https://youtu.be/ZJoBy2c1dPk
  Do you like my videos? You can support the channel:
  https://patreon.com/acrobotic
  https://paypal.me/acrobotic
  ------------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 
  https://acrobotic.com/
  https://amazon.com/acrobotic
  ------------------------------------------------------------------------------
  License:
  Please see attached LICENSE.txt file for details.
------------------------------------------------------------------------------*/
/*

const char* createTableUser = "CREATE TABLE User (\
    ID        INTEGER  PRIMARY KEY \
                       UNIQUE \
                       NOT NULL,\
    LastName  VARCHAR2 NOT NULL,\
    FirstName VARCHAR2 NOT NULL,\
    Username  VARCHAR2 NOT NULL\
                       UNIQUE,\
    Password  VARCHAR2 NOT NULL,\
    TagNumber VARCHAR2 UNIQUE,\
    Credit    REAL\
);";
*/

//const char* createTableUser = "CREATE TABLE User (ID INTEGER PRIMARY KEY, LastName VARCHAR2(16) NOT NULL, FirstName VARCHAR2(16) NOT NULL, Username VARCHAR2(16) NOT NULL UNIQUE, Password VARCHAR2(16) NOT NULL, TagNumber VARCHAR2(16) UNIQUE, Credit REAL);";

const char* createTableUser = "CREATE TABLE User (ID INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Username VARCHAR2(16) NOT NULL, Password VARCHAR2(12) NOT NULL, TagNumber VARCHAR2(16), Credit REAL);";
const char* insertUser = "INSERT INTO User (FirstName, LastName, Username, Password) VALUES ('Ion', 'Vasilescu', 'ion.v', 'abc');";
const char* insertUser2 = "INSERT INTO User (FirstName, LastName, Username, Password) VALUES ('Maria', 'Ionescu', 'm.ionescu', '346456');";
const char* sqlAllUsers = "SELECT * from User;";

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#include <FS.h>
extern "C" {
#include "user_interface.h"
}
   int rc;
   sqlite3 *db1;
   void SaveToDB();
//CONNECT TO LOCAL NETWORK
ESP8266WebServer server;
char* ssid = "Telekom-00E3";
char* password = "6DKY7ELEXZ9SZ";
//open file in SPIFFS memory
void GetIndexPage(){
  File file  = SPIFFS.open("/index.html", "r");
  server.streamFile(file, "text/html");
  }

void GetUsers(){
  File file  = SPIFFS.open("/index.html", "r");
  server.streamFile(file, "text/html");
  }  
  
const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: \r\n", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}
int db_open(const char *filename, sqlite3 **db) {
   int rc = sqlite3_open(filename, db);
   if (rc) {
       Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
       return rc;
   } else {
       Serial.printf("Opened database successfully\n");
   }
   return rc;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   return rc;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");
  SPIFFS.begin();
  

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  //for removing files use he commend bellow
  //SPIFFS.remove("/index.htm");
  server.on("/",GetIndexPage);
  server.on("/users", GetUsers);
  server.begin();


   system_update_cpu_freq(SYS_CPU_160MHZ);

   if (!SPIFFS.begin()) {
       Serial.println("Failed to mount file system");
       return;
   }
// see all files in SPIFFS memory   
  Dir dir = SPIFFS.openDir ("");
  while (dir.next ()) {
    Serial.println (dir.fileName ());
    Serial.println (dir.fileSize ());
  }
// remove existing file
  // SPIFFS.remove("/test1.db");
   SPIFFS.remove("/VendingMachineDB.db");

   sqlite3_initialize();

   // Open databases
   File db_file_obj_1;
   vfs_set_spiffs_file_obj(&db_file_obj_1);
   if (db_open("/FLASH/VendingMachineDB.db", &db1))
       return;

    //CREATE DB
   //rc = db_exec(db1, "CREATE TABLE Users ( FirstName VARCHAR2(16), LastName VARCHAR2(16),Username VARCHAR2(16), Password VARCHAR2(12) NOT NULL);");
   rc = db_exec(db1, createTableUser);
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
     //  sqlite3_close(db2);
       return;
   }

   rc = db_exec(db1, insertUser);
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       return;
   }


   rc = db_exec(db1, insertUser2);
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       return;
   }
   
   //SELECT USERS
      rc = db_exec(db1, sqlAllUsers);
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
     //  sqlite3_close(db2);
       return;
   }
  
}


void loop()
{
  //server.handleClient();
}

//------------------------------------FUNCTIONS------------------------------------------
void SaveToDB(){
    Serial.println("\nInserted in db!");
  String FirstName = server.arg("FirstName");
  String LastName = server.arg("LaseName");
  String Username = server.arg("Username");
  String Password = server.arg("Password");
   //INSERT VALUES IN DB
   rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password) VALUES ('Ion', 'Vasilescu', 'ion.v', 'abc');");
   if (rc != SQLITE_OK) {
       sqlite3_close(db1);
       return;
   }
  //send a message to the user
  server.send(200, "text/html", "ok");

  }
