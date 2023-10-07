const char* createTableUser = "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL,PhoneNumber VARCHAR2(10), Email VARCHAR2(16), TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);";
const char* insertUser = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);";
const char* insertUser2 = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (29210144293, 'Maria', 'Ionescu', '346456',2.20, 0, 1 );";
const char* sqlAllUsers = "SELECT * FROM Users;";

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
File db_file_obj_1;
int rc;
sqlite3* db1;
void SaveToDB();
char buffer[1000];
char jsonFormat[1000];
String response = "{";
//CONNECT TO LOCAL NETWORK
ESP8266WebServer server(80);
char* ssid = "Telekom-167024";
char* password = "fU5vLVbCFfxga";
//open file in SPIFFS memory
void GetIndexPage()
{
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
}


void Test() {
    String UserCNP = server.arg("Username");
    String UserPassword = server.arg("Password");
    sprintf(buffer, "SELECT CNP, FirstName, LastName from Users WHERE (CNP=%s) AND (Password='%s');", UserCNP.c_str(), UserPassword.c_str());
    Serial.print("\nbuffer: ");
    Serial.print(buffer);
    /*vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open("/FLASH/VendingMachineDB.db", &db1))
        return;
        */
    const  char* persQuery = buffer;
    rc = db_exec(db1, persQuery);
    //rc = db_exec(db1, persQuery);
    
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    Serial.printf("jsonFormat: %s", jsonFormat);
     server.send(200, "application/json",jsonFormat );
}

void LogIn()
{

    String UserCNP = server.arg("Username");
    String UserPassword = server.arg("Password");
    //   sprintf(buffer, "SELECT CNP, FirstName, LastName from Users WHERE (CNP='%s') AND (Password='%s');", UserCNP.c_str(), UserPassword.c_str());
    sprintf(buffer, "SELECT * from Users WHERE (CNP='%s') AND (Password='%s');", UserCNP.c_str(), UserPassword.c_str());
    Serial.print("\nbuffer: ");
    Serial.print(buffer);
    const  char* persQuery = buffer;
    rc = db_exec(db1, persQuery);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
}

const char* data = "Callback function called";
static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    Serial.printf("%s: \r\n", (const char*)data);
    bool functionCalled = true;
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        //"\"" + azColName[i] + "\":" + " \"" + argv[i] + "\","; 
        if (argv[i] != NULL)
        {
         snprintf(jsonFormat , "{\"CNP"\:\%d\, \"FirstName"\: \%s\, \"LastName"\:\%s\,\"PhoneNumber"\: \%s\,\"Email"\:\%s\, \"isAdmin"\: \%d\, \"Credit"\: \%f\,\"TagNumber"\: \%s\, \"isActive"\: \%d\}", argv[0], argv[1], argv[2], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
        }
    
    Serial.print("Printing response: ");
    Serial.print(response);
    if (functionCalled) {
        response = "{";
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
    Serial.println("\ndb_exec");
    int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    Serial.println(millis());
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
    server.on("/Test", Test);
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
     SPIFFS.remove("/test1.db");
    SPIFFS.remove("/VendingMachineDB.db");

    sqlite3_initialize();

    // Open databases
    vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open("/FLASH/VendingMachineDB.db", &db1))
        return;

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

  // LogIn();
}

int checkMem = 0;
void loop()
{
    server.handleClient();
/*
    if (!(checkMem % 1000000))
    {
       Serial.printf("\r\nFree mem = %d", ESP.getFreeHeap());
       Test();
    }
    checkMem++;
*/
}
