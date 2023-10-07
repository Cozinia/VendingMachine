
#pragma page_size=512;
#include <cont.h>
// VACUUM;
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <vfs.h>
#include <SPI.h>
#include <FS.h>
   File db_file_obj_1;
sqlite3* db;
char buffer[1000];
int rc;

//Creating table
const char* createTableUser = "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL,PhoneNumber VARCHAR2(10), Email VARCHAR2(16), TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);";
//Inserting users
const char* insertUser = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);";
const char* insertUser2 = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (29210144293, 'Maria', 'Ionescu', '346456',2.20, 0, 1 );";
//showing all users in db
const char* sqlAllUsers = "SELECT * FROM Users;";

void Test() {
    Serial.print("Starting Test...\r\n");
//   vfs_set_spiffs_file_obj(&db_file_obj_1);
//   if (db_open("/FLASH/test1.db", &db))
//       return;
    
    sprintf(buffer, "SELECT * FROM Users;");
    Serial.print("\nbuffer: ");
    Serial.print(buffer);
    const  char* persQuery = buffer;
    rc = db_exec(db, sqlAllUsers);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }
    //sqlite3_close(db);
}

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
   Serial.println("before sqlite3_exec");
   //problem seems to appear when executing the instruction bellow
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   Serial.println("after sqlite3_exec");
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

void setup() {

   Serial.begin(115200);
 Serial.printf("Starting New Test...%d", CONT_STACKSIZE);
   //system_update_cpu_freq(SYS_CPU_160MHZ);

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

   vfs_set_spiffs_file_obj(&db_file_obj_1);
   if (db_open("/FLASH/test1.db", &db))
       return;
  
    //CREATE DB
    rc = db_exec(db, createTableUser);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }
    //Inserting first user
    rc = db_exec(db, insertUser);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }

     //Inserting second user
    rc = db_exec(db, insertUser2);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }

    //SELECT USERS
    rc = db_exec(db, sqlAllUsers);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    } 
    //sqlite3_close(db);
  //here works
   Test();


}

int checkMem = 0;
void loop() {
  //here it doesn't work
      if (!(checkMem % 1000000))
    {
       Serial.printf("\r\nFree mem = %d", ESP.getFreeHeap());
       Test();
    }
    checkMem++;
    
}
