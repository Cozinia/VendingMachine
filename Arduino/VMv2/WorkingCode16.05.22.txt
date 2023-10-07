/*
    This creates two empty databases, populates values, and retrieves them back
    from the SPIFFS file system.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#include <FS.h>
extern "C" {
#include "user_interface.h"
}
//#include <ESP8266WiFi.h>

sqlite3* db;
File db_file_obj_1;

const char* const dbPath = "/vm.db";
const char* const dbFile = "/FLASH/vm.db";
const char* const data = "Callback function called";

const char* const createTableUser = "CREATE TABLE User (CNP INTEGER(13) PRIMARY KEY, Name TEXT, Password TEXT, Tag TEXT, Credit NUMERIC, Admin INTEGER);";
const char* const createTableProduct = "CREATE TABLE Product (ID INTEGER PRIMARY KEY, Name TEXT, Price NUMERIC, Position INTEGER, Quantity INTEGER);";
const char* const createTableTransaction = "CREATE TABLE Log (ID INTEGER PRIMARY KEY, Timestamp TEXT, Type INTEGER, Value NUMERIC, Quantity INTEGER, CNP NUMBER(13), ProductID NUMBER(5));";

const char* const insertUser1 = "INSERT INTO User(CNP, Name, Password, Tag, Credit, Admin) VALUES (19712073264, 'Ion Vasilescu', 'abc', '22918314235', 2.40, 1);";
const char* const insertUser2 = "INSERT INTO User(CNP, Name, Password, Tag, Credit, Admin) VALUES (28604157326, 'Maria Popescu', '1234', '167116175', 5.20, 0);";
const char* const insertUser3 = "INSERT INTO User(CNP, Name, Credit, Admin) VALUES (0, 'Anonymus', 0, 0);";

const char* const insertProduct1 = "INSERT INTO Product(ID, Name, Price, Position, Quantity) VALUES (1, 'Snikers', 2.5, 3, 5)";
const char* const insertProduct2 = "INSERT INTO Product(ID, Name, Price, Position, Quantity) VALUES (2, 'Mars', 3, 2, 3)";
const char* const insertProduct3 = "INSERT INTO Product(ID, Name, Price, Position, Quantity) VALUES (3, 'Twix', 1.25, 4, 4)";
const char* const insertProduct4 = "INSERT INTO Product(ID, Name, Price, Position, Quantity) VALUES (4, 'Oreo', 3.40, 1, 2)";

const char* const insertLog1 = "INSERT INTO Log(ID, Timestamp, Type, Value, Quantity) VALUES (12, '2022-02-16 14:12:48', 1, 2.5, 1)";
const char* const insertLog2 = "INSERT INTO Log(ID, Timestamp, Type, Value, Quantity) VALUES (43, '2022-01-13 10:50:23', 2, 10, 1)";
const char* const insertLog3 = "INSERT INTO Log(ID, Timestamp, Type, Value, Quantity) VALUES (61, '2022-04-19 16:24:13', 1, 6.20, 2)";

static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    Serial.printf("%s: ", (const char*)data);
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

char* zErrMsg = 0;
int db_exec(sqlite3* db, const char* sql)
{
    //   System.out.println(sql);
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

    Serial.begin(9600);


    int rc;

    system_update_cpu_freq(SYS_CPU_160MHZ);
    //WiFiOff();

    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }

    // list SPIFFS contents
    
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
        String fileName = dir.fileName();
        size_t fileSize = dir.fileSize();
        Serial.printf("FS File: %s, size: %ld\n", fileName.c_str(), (long)fileSize);
        //SPIFFS.remove(fileName.c_str());
    }
    Serial.printf("\n");

    // remove existing file
    SPIFFS.remove(dbPath);
    
    sqlite3_initialize();

    // Open databases

    vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open(dbFile, &db))
        return;
    
    Serial.printf("\n2");
    rc = db_exec(db, createTableProduct);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    
    //Create tables
    Serial.printf("\n1");
    rc = db_exec(db, createTableUser);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    

    //Create tables
    Serial.printf("\n3");
    rc = db_exec(db, createTableTransaction);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    
    //Insert 3 Users
    Serial.printf("\n4");
    rc = db_exec(db, insertUser1);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    
    Serial.printf("\n5");
    rc = db_exec(db, insertUser2);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }

    Serial.printf("\n6");
    rc = db_exec(db, insertUser3);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    
    //Insert 4 products
    Serial.printf("\n7");
    rc = db_exec(db, insertProduct1);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    Serial.printf("\n8");
    rc = db_exec(db, insertProduct2);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    Serial.printf("\n9");
    rc = db_exec(db, insertProduct3);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    Serial.printf("\n10");
    rc = db_exec(db, insertProduct4);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    //Insert 3 Logs
    Serial.printf("\n11");
    rc = db_exec(db, insertLog1);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    Serial.printf("\n12");
    rc = db_exec(db, insertLog2);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    Serial.printf("\n13");
    rc = db_exec(db, insertLog3);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);

        return;
    }
    sqlite3_close(db);
    

}

void loop()
{
}