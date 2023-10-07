//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

const char* createTableUser = "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL,PhoneNumber VARCHAR2(10), Email VARCHAR2(16), TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);";
const char* insertUser = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);";
const char* insertUser2 = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (29210144293, 'Maria', 'Ionescu', '346456',2.20, 0, 1 );";
const char* sqlAllUsers = "SELECT * FROM Users;";

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
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
sqlite3* db1;

AsyncWebServer server(80);

const char* ssid = "Telekom-00E3";
const char* password = "6DKY7ELEXZ9SZ";

const char* PARAM_MESSAGE = "message";

    char* res = "{\"Token\": \"102\", \"User\": {\"ID\":1, \"FirstName\": \"Ion\", \"LastName\":\"Vasilescu\", \"Username\":\"ion.vasilescu\", \"isAdmin\": 1, \"Credit\": 8.40, \"TagNumber\": \"#165652JJFV\", \"isActive\": 1} }";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void GetIndexPage()
{
    File file = SPIFFS.open("/index.html", "r");
   // server.streamFile(file, "text/html");
}

void LogIn(){
  //send json to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200,  "application/json", res);
    });
  }

const char* data = "Callback function called";
static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    Serial.printf("%s: \r\n", (const char*)data);
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        //if (azColName[i] == "CNP") {
        //    Serial.print("\nPrinting CNP: ");
        //    Serial.print(argv[i] ? argv[i] : "NULL");
        //    Serial.print("\n");
        //}
    }
    Serial.printf("\n");
    return 0;
}

int db_open(const char* filename, sqlite3** db)
{
    int rc = sqlite3_open(filename, db);
    if (rc)
    {
        Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    else
    {
        Serial.printf("Opened database successfully\n");
    }
    return rc;
}

char* zErrMsg = 0;
int db_exec(sqlite3* db, const char* sql)
{
    Serial.print("Printing SQL: ");
    Serial.print(sql);
    Serial.print("\n");
    long start = micros();
    int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        Serial.printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        Serial.printf("Operation done successfully\n");
    }
    Serial.print(F("Time taken:"));
    Serial.println(micros() - start);
    return rc;
}

void setup() {
    
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("\nIP Address: ");
    Serial.println(WiFi.localIP());
    SPIFFS.begin();

     server.on("/", GetIndexPage);
    server.on("/LogIn", LogIn);
    server.begin();


    system_update_cpu_freq(SYS_CPU_160MHZ);

    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }
    // see all files in SPIFFS memory   
    Dir dir = SPIFFS.openDir("");
    while (dir.next())
    {
        Serial.println(dir.fileName());
        Serial.println(dir.fileSize());
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
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }

    rc = db_exec(db1, insertUser);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }


    rc = db_exec(db1, insertUser2);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

    //SELECT USERS
    rc = db_exec(db1, sqlAllUsers);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }
    
/*
    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message);
    });
*/
    server.onNotFound(notFound);

    server.begin();
}

void loop() {
}
