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
#include <ESP8266WiFi.h>

const char* data = "Callback function called";
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

    Serial.begin(9600);
    sqlite3* db1;
    //  sqlite3 *db2;
    int rc;

    system_update_cpu_freq(SYS_CPU_160MHZ);

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
    }
    Serial.printf("\n");

    // remove existing file
   // SPIFFS.remove("/test1.db");
   // SPIFFS.remove("/VendingMachineDB.db");

    sqlite3_initialize();

    // Open databases
    File db_file_obj_1;
    vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open("/FLASH/VendingMachineDB.db", &db1))
        return;

    //CREATE USERS DB
    rc = db_exec(db1, "CREATE TABLE Users (ID INTEGER  PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16),username VARCHAR2(16), Password VARCHAR2(16) NOT NULL, IsAdmin INTEGER, Credit   FLOAT(4,2), TagNumber VARCHAR(10));");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }
    //CREATE PRODUCTS DB
    rc = db_exec(db1, "CREATE TABLE Products (ID INTEGER  PRIMARY KEY, Location INTEGER, Name VARCHAR2(16),Stock INTEGER, InserationDate DATE, SoldOutDate DATE));");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }
    //CREATE ACTION DB
    rc = db_exec(db1, "CREATE TABLE Actions (ID INTEGER  PRIMARY KEY, Timestamp TIMESTAMP NOT NULL, TypeID INTEGER NOT NULL,UserID INTEGER NOT NULL REFERENCES Users (ID) ON UPDATE CASCADE, ProductID INTEGER REFERENCES Products (ID) ON UPDATE CASCADE, Credit   FLOAT(4,2), Quantity INTEGER);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }


    //INSERT VALUES IN DB
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ('Ion', 'Vasilescu', 'ion.v', 'abc', 0, 8.40, '165652JJFV');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users VALUES (2, 'John', 'Burn', 'john.burn', 'johnny12', 8.40);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users VALUES (3, 'Oscar', 'Care', 'oscar224', 'puppy597', 14.20);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users VALUES (4, 'Katy', 'Parrish', 'katy_pet_lover226', 'husky@658', 2.80);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users VALUES (5, 'Tamara', 'Taylor', 'the_real_tamara15', 'usbc!*561', 4.95);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //SELECT
    rc = db_exec(db1, "SELECT * FROM Users;");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //CLOSE DB
    sqlite3_close(db1);

}

void loop()
{
}