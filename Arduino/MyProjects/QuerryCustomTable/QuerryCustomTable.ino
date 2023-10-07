#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
extern "C" {
#include "user_interface.h"
}
//-------Declarations--------------------
char buffer[1000];
   sqlite3 *db1;
   int rc;

//CONNECT TO LOCAL NETWORK
ESP8266WebServer server(80);
char* ssid = "Telekom-00E3";
char* password = "6DKY7ELEXZ9SZ";

//open file in SPIFFS memory
void GetIndexPage()
{
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
}

//Creating table
const char* createTableUser = "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL,PhoneNumber VARCHAR2(10), Email VARCHAR2(16), TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);";
//Inserting users
const char* insertUser = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);";
const char* insertUser2 = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (29210144293, 'Maria', 'Ionescu', '346456',2.20, 0, 1 );";
//showing all users in db
const char* sqlAllUsers = "SELECT * FROM Users;";

//Takes data from the form 
void Test() {
    /* String UserCNP = "19712073264";
     String UserPassword = "abc";*/
    String UserCNP = server.arg("Username");
    String UserPassword = server.arg("Password");
  //  sprintf(buffer, "SELECT CNP, FirstName, LastName from Users WHERE (CNP='%s') AND (Password='%s');", UserCNP.c_str(), UserPassword.c_str());
//    const  char* persQuery = buffer.c_str();
   String command = "SELECT CNP, FirstName, LastName from Users WHERE (CNP='" + UserCNP + "') AND (Password='" + UserPassword + "');";
   const char* sql_Command = command.c_str();
    rc = db_exec(db1,sql_Command);
    delay(100);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    delay(500);
}

//Create db function---------
void CreateDB(){
    //CREATE DB
    rc = db_exec(db1, createTableUser);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }
}

//Insert user1
void InsertUser1(){
    //Inserting first user
    rc = db_exec(db1, insertUser);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
}
//Insert user2
void InsertUser2(){
     //Inserting second user
    rc = db_exec(db1, insertUser2);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
}

//Select all users
void SelectAllUsers(){
    //SELECT USERS
    rc = db_exec(db1, sqlAllUsers);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }  
}


//----Callback function
const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}

//-----Open db function------------
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

//-------Exec querry function------
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

//----Setup function
void setup() {

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
    server.on("/Test", Test);
    server.begin();


   system_update_cpu_freq(SYS_CPU_160MHZ);


   if (!SPIFFS.begin()) {
       Serial.println("Failed to mount file system");
       return;
   }

   // list SPIFFS contents
   Dir dir = SPIFFS.openDir("/");
   while (dir.next()) {    
       String fileName = dir.fileName();
       size_t fileSize = dir.fileSize();
       Serial.printf("FS File: %s, size: %ld\n", fileName.c_str(), (long) fileSize);
   }
   Serial.printf("\n");

   // remove existing file
   SPIFFS.remove("/test1.db");

   sqlite3_initialize();

   // Open databases
   File db_file_obj_1;
   vfs_set_spiffs_file_obj(&db_file_obj_1);
   if (db_open("/FLASH/test1.db", &db1))
       return;

  //Create table
  CreateDB();
  //Insert user1
  InsertUser1();
  //Insert user2
  InsertUser2();
  //Select all users
  SelectAllUsers();
}

void loop() {
  //handle request from client
   server.handleClient();
}
