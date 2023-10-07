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

char sqlResult[100];
char* zErrMsg = 0;
const char* data = "Callback function called";
/*Create db*/
//const char* createTableUser = "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL,PhoneNumber VARCHAR2(10), Email VARCHAR2(16), TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);";
//const char* insertUser = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);";
//const char* insertUser2 = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (29210144293, 'Maria', 'Ionescu', '346456',2.20, 0, 1 );";
const char* sqlAllUsers = "SELECT * FROM Users;";
int rc;
sqlite3* db1;
void SaveToDB();

//initialise server
ESP8266WebServer server;
char* ssid = "Telekom-00E3";
char* password = "6DKY7ELEXZ9SZ";
//open file in SPIFFS memory
void GetIndexPage()
{
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
}


int DbExecQuery(sqlite3* db, const char* sql, sqlite3_callback callbackFunc, void* data)
{
    Serial.println(sql);
    long start = micros();
    int rc = sqlite3_exec(db, sql, callbackFunc, (void*)data, &zErrMsg);
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


static int callbackLogin(void* data, int argc, char** argv, char** azColName)
{
    int i;
    if (argc >= 1)
    {
        sprintf((char*)data, "{\"Token\": \"%s\", \"User\": {\"CNP\":%s, \"FirstName\": \"%s\", \"LastName\":\"%s\"} }", argv[0], argv[0], argv[1], argv[2]);
        //    sprintf(buffer,"{\"Token\": \"%s\", \"User\": {\"CNP\":%s, \"FirstName\": \"%s\", \"LastName\":\"%s\", \"isAdmin\": 1, \"Credit\": %f, \"TagNumber\": \"%s\", \"isActive\": %s} }", argv[0], argv[1], argv[2] );
    }
    Serial.printf("%s: \r\n", (const char*)data);

    Serial.printf("\n");
    return 0;
}

static int callbackUsers(void* data, int argc, char** argv, char** azColName)
{
    int i;
    Serial.printf("%s: \r\n", (const char*)data);
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    Serial.printf("\n");
    return 0;
}


int LogIn()
{
    char sql[200];


    //take credentials from web page
    String CNP = server.arg("Username");
    String UserPassword = server.arg("Password");
    Serial.print("\nCNP: ");
    Serial.print(CNP);
    Serial.print("\nUser's Password is ");
    Serial.print(UserPassword);
    sprintf(sql, "SELECT CNP, FirstName, LastName from User WHERE (CNP='%d') AND (Password='%s');", CNP.c_str(), UserPassword.c_str());

    sqlResult[0] = 0;
    DbExecQuery(db1, sql, callback, sqlResult);

    //char* res = "{\"Token\": \"101\", \"User\": {\"ID\":1, \"FirstName\": \"Ion\", \"LastName\":\"Vasilescu\", \"Username\":\"ion.vasilescu\", \"isAdmin\": 1, \"Credit\": 8.40, \"TagNumber\": \"#165652JJFV\", \"isActive\": 1} }";

    if (sqlResult[0])
    {
        server.send(200, "application/json", sqlResult);
    }
    else
    {
        //error code that indicates user not found
        server.send(602, "text/plain");
    }
}


static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    Serial.printf("%s: \r\n", (const char*)data);
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
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


int db_exec(sqlite3* db, const char* sql)
{
    Serial.println(sql);
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

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting...");
    SPIFFS.begin();


    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    //for removing files use he commend bellow
    //SPIFFS.remove("/index.htm");
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
     //  SPIFFS.remove("/VendingMachineDB.db");

    sqlite3_initialize();

    // Open databases
    File db_file_obj_1;
    vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open("/FLASH/VendingMachineDB.db", &db1))
    {
        Serial.println("Db open failed!\r\n");
        return;
    }
    /*
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
       */
       //SELECT USERS
    rc = db_exec(db1, sqlAllUsers);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }


}


void loop()
{
    server.handleClient();
}
//------------------------------------FUNCTIONS------------------------------------------
void SaveToDB()
{
    Serial.println("\nInserted in db!");
    String FirstName = server.arg("FirstName");
    String LastName = server.arg("LaseName");
    String Username = server.arg("Username");
    String Password = server.arg("Password");
    //INSERT VALUES IN DB
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password) VALUES ('Ion', 'Vasilescu', 'ion.v', 'abc');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //send a message to the user
    server.send(200, "text/html", "ok");

}
