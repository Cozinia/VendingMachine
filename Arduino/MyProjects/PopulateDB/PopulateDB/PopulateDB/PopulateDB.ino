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
   //SPIFFS.remove("/VendingMachineDB.db");
   // SPIFFS.remove("/index.html");
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

    sqlite3_initialize();

    // Open databases
    File db_file_obj_1;
    vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open("/FLASH/VendingMachineDB.db", &db1))
        return;
    
    //CREATE USERS TABLE
    rc = db_exec(db1, "CREATE TABLE Users(id integer PRIMARY KEY, FirstName text not null, LastName text not null, Username text not null, Password text not null, IsAdmin integer not null,Credit float(4,2), TagNumber text);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }
    //CREATE PRODUCTS TABLE
    rc = db_exec(db1, "CREATE TABLE Products(id integer PRIMARY KEY,Location integer not null, Name text not null,Stock integer not null, InserationDate date not null, SoldOutDate date);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }
    
    //CREATE ACTIONS TABLE
    rc = db_exec(db1, "create table Actions (ID INTEGER,Timestamp TIMESTAMP NOT NULL, TypeID INTEGER NOT NULL, UserID INTEGER,ProductID  INTEGER, Credit FLOAT(4,2), Quantity INTEGER ,FOREIGN KEY (UserID) REFERENCES Users(id),FOREIGN KEY (ProductID) REFERENCES Products(id),PRIMARY KEY (ID, UserID));");
    rc = db_exec(db1, "create table Actions (ID INTEGER,Timestamp TIMESTAMP NOT NULL, TypeID INTEGER NOT NULL, UserID INTEGER,ProductID  INTEGER, Credit FLOAT(4,2), Quantity INTEGER);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        //  sqlite3_close(db2);
        return;
    }

    
    //INSERT VALUES IN USERS TABLE
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ('Ion', 'Vasilescu', 'ion.vasilescu', 'abc', 0, 8.40, '#165652JJFV');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ('Maria', 'Popescu', 'maria.popescu', 'lusindx15', 0, 3.80, '#LKBDE58CDW');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin) VALUES ('Cozinia', 'Motronu', 'cozinia.motronu', 'test12', 1);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ('Mirela', 'Ionescu', 'mirela.ionescu', 'uhvihbs@562', 0, 2.80, '#112LNUCDISDG64516');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ( 'Bogdan', 'Stoica', 'bogdan.stoica', 'usbc!*561', 0, 4.95, '#112UEROVBEURSY');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ( 'Cornelia', 'Mihai', 'cornelia.mihai', 'iovw#865', 0, 5.20, '#112LOIUR96FE');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ( 'Alesia', 'Bratu', 'alesia.bratu', 'luhf$lhbve58', 0, 8.15, '#112LUIYVFOE3576');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ( 'Oana', 'Popescu', 'oana.popescu', 'usbc!*561', 0, 10.05, '#116UIYVOVEW6486FE');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ( 'Iulia', 'Ene', 'iulia.ene', ';oihdfW$31', 0, 2.20, '#116;OIUDF257FVD');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //SELECT * FROM USERS
    rc = db_exec(db1, "SELECT * FROM Users;");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //INSERT INTO PRODUCTS TABLE
    rc = db_exec(db1, "INSERT INTO Products (Location, Name, Stock, InserationDate) VALUES ( 1, 'Snickers', 5, '2022-02-17');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Products (Location, Name, Stock, InserationDate) VALUES ( 2, 'Skittles', 6, '2022-02-18');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Products (Location, Name, Stock, InserationDate) VALUES ( 3, '7Days Chocolate', 5, '2022-02-15');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Products (Location, Name, Stock, InserationDate) VALUES ( 4, 'CocaCola', 4, '2022-02-16');");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //SELECT * FROM PRODUCTS
    rc = db_exec(db1, "SELECT * FROM Products;");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    
    
    //INSERT INTO ACTIONS TABLE
    rc = db_exec(db1, "INSERT INTO Actions (Timestamp, TypeID, UserID, ProductID, Credit, Quantity) VALUES ('2022-02-16 14:12:48' , 1, 3, 2, 2.50, 1);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Actions (Timestamp, TypeID, UserID, ProductID, Credit, Quantity) VALUES ('2022-03-18 08:34:06' , 1, 4, 3, 3.00, 1);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Actions (Timestamp, TypeID, UserID, ProductID, Credit, Quantity) VALUES ('2022-01-13 18:48:16' , 1, 9, 1, 1.50, 1);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Actions (Timestamp, TypeID, UserID, Credit) VALUES ('2022-02-16 14:12:48' , 2, 4, 2.50);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    rc = db_exec(db1, "INSERT INTO Actions (Timestamp, TypeID, UserID, ProductID, Credit, Quantity) VALUES ('2022-01-13 10:50:23' , 1, 5, 2, 2.50, 1);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
    //SELECT * FROM ACTIONS
    rc = db_exec(db1, "SELECT * FROM Actions;");
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
