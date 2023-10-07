/*
//Users
const char* const createTableUser = "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL, TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);";
//const char* const createTableUser = "CREATE TABLE Users(id integer PRIMARY KEY, FirstName text not null, LastName text not null, Username text not null, Password text not null, IsAdmin integer not null,Credit float(4,2), TagNumber text);";
const char* const insertUser = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);";
//const char* const insertUser = "INSERT INTO Users (FirstName, LastName, Username, Password, IsAdmin, Credit, TagNumber) VALUES ('Ion', 'Vasilescu', 'ion.vasilescu', 'abc', 0, 8.40, '#165652JJFV');";

const char* insertUser2 = "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES  (1, 'Anonymus', 'Anonymus', '1', 0, 0, 1 );";

//Products
const char* const createTableProducts = "CREATE TABLE Product (ID INTEGER PRIMARY KEY, Name VARCHAR2(30),Price NUMBER(2,1), Position NUMBER(1), isActive NUMBER(1) NOT NULL, Quantity NUMBER(2) NOT NULL);";
//const char* const createTableProducts = "CREATE TABLE Products(id integer PRIMARY KEY,Location integer not null, Name text not null,Stock integer not null, InserationDate date not null, SoldOutDate date);";
//Actions
const char* const createTableTransactions = "CREATE TABLE Transaction(ID INTEGER PRIMARY KEY, Timestamp TIMESTAMP NOT NULL, TypeID NUMBER(1) NOT NULL, Credit NUMBER(4,2), Quantity NUMBER(2));";
//const char* const createTableTransactions = "CREATE TABLE Actions (ID INTEGER PRIMARY KEY,Timestamp TIMESTAMP NOT NULL, UserID INTEGER,ProductID  INTEGER, Credit FLOAT(4,2), Quantity INTEGER);";

//const char* const pragma = "PRAGMA page_size=512; VACUUM;";
//Products
//const char* createTableProducts = "CREATE TABLE Product (ID INTEGER PRIMARY KEY, Name VARCHAR2(30),Price NUMBER(2,1), Position NUMBER(1), isActive NUMBER(1) NOT NULL, Quantity NUMBER(2) NOT NULL);";
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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
#define BufferSize 500
char tempBuffer[BufferSize];
char buffer[BufferSize];
int i;
//CONNECT TO LOCAL NETWORK
ESP8266WebServer server(80);

const char* const ssid = "Telekom-167024";
const char* const password = "fU5vLVbCFfxga";

//char* ssid = "Telekom-00E3";
//char* password = "6DKY7ELEXZ9SZ";
//open file in SPIFFS memory


void GetIndexPage()
{
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
}


void RunCommand()
{
    buffer[0] = NULL;
    String SQLCommand = server.arg("SQLCommand");

    strcat(buffer, "[");
    Serial.print("\nbuffer: ");
    Serial.print(buffer);
    const  char* persQuery = SQLCommand.c_str();
    rc = db_exec(db1, persQuery);

    if (rc != SQLITE_OK)
    {
        strcat(buffer, "]");
        server.send(200, "application/json", buffer);
        return;
    }
    int len = strlen(buffer);
    if (buffer[len - 2] == ',')
    {
        buffer[len - 2] = ']';
    }
    else
    {
        strcat(buffer, "]");
    }
    Serial.printf("buffer: %s", buffer);

    server.send(200, "application/json", buffer);

}

void LogIn()
{
    tempBuffer[0] = NULL;
    buffer[0] = NULL;
    strcat(buffer, "[");
    String UserCNP = server.arg("Username");
    String UserPassword = server.arg("Password");
    sprintf(tempBuffer, "SELECT Name from User WHERE (CNP='%s') AND (Password='%s');", UserCNP.c_str(),UserPassword.c_str());
    Serial.print("\nbuffer: ");
    Serial.print(tempBuffer);
    const  char* userQuery = tempBuffer;
    rc = db_exec(db1, userQuery);

    int len = strlen(buffer);
    if (buffer[len - 2] == ',')
    {
        buffer[len - 2] = ']';
    }
    else
    {
        strcat(buffer, "]");
    }
    Serial.printf("buffer: %s", buffer);

    if (buffer == "[]")
    {
        server.send(602, "application/json", buffer);
    }

    if (rc != SQLITE_OK)
    {
        strcat(buffer, "]");
        server.send(602, "application/json", buffer);
        return;
    }
    else {
        server.send(200, "application/json", buffer);
    }
}

//void Motor1()
//{
//    Serial.print("\nRotate motor m1");
//   // m1.Rotate(true, 200);
//    server.send(200, "text/plain", "Rotate motor 1");
//
//    for (int i = 30; i >= 0; i--)
//    {
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("Test motor 1S");
//    }
//    lcd.clear();
//   
//}
//
//void Motor2()
//{
//    Serial.print("\nRotate motor m2");
//   // m2.Rotate(true, 200);
//
//    //m2.Rotate(false, 200);
//
//    server.send(200, "text/plain", "Rotate motor 2");
//    for (int i = 30; i >= 0; i--)
//    {
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("Test motor 2");
//    }
//        lcd.clear();
//}
//
//void Motor3()
//{
//    Serial.print("\nRotate motor m3");
//   // m3.Rotate(false, 200);
//    server.send(200, "text/plain", "Rotate motor 3");
//    for (int i = 30; i >= 0; i--)
//    {
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("Test motor 3");
//    }
//    lcd.clear();
//
//}
/*
void Products() {
        buffer[0] = NULL;
        strcat(buffer, "[");
        rc = db_exec(db1, "SELECT * FROM Product");
    
        int len = strlen(buffer);
        if (buffer[len - 2] == ',')
        {
            buffer[len - 2] = ']';
        }
        else
        {
            strcat(buffer, "]");
        }
        Serial.printf("buffer: %s", buffer);
        if (rc != SQLITE_OK)
        {
            strcat(buffer, "]");
            //Cannot found users
            server.send(603, "application/json", buffer);
            return;
        }
        else
        {
            server.send(200, "application/json", buffer);
        }
}*/

//void Users() {
//    buffer[0] = NULL;
//    strcat(buffer, "[");
//    rc = db_exec(db1, sqlAllUsers);
//
//    int len = strlen(buffer);
//    if (buffer[len - 2] == ',')
//    {
//        buffer[len - 2] = ']';
//    }
//    else
//    {
//        strcat(buffer, "]");
//    }
//    Serial.printf("buffer: %s", buffer);
//    if (rc != SQLITE_OK)
//    {
//        strcat(buffer, "]");
//        //Cannot found users
//        server.send(603, "application/json", buffer);
//        return;
//    }
//    else
//    {
//        server.send(200, "application/json", buffer);
//    }
//    
//}
//
//void EnableUser() {
//   
//    tempBuffer[0] = NULL;
//    buffer[0] = NULL;
//    strcat(buffer, "[");
//    String UserCNP = server.arg("CNP");
//    sprintf(tempBuffer, "UPDATE Users SET isActive = '1' WHERE CNP = %s; ", UserCNP.c_str());
//    Serial.print("\nbuffer: ");
//    Serial.print(tempBuffer);
//    const  char* userQuery = tempBuffer;
//    rc = db_exec(db1, userQuery);
//    if (rc != SQLITE_OK)
//    {
//        strcat(buffer, "]");
//        server.send(200, "application/json", buffer);
//        return;
//    }
//    int len = strlen(buffer);
//    if (buffer[len - 2] == ',')
//    {
//        buffer[len - 2] = ']';
//    }
//    else
//    {
//        strcat(buffer, "]");
//    }
//    server.send(200, "application/json", buffer);
//
//}
//
//void DisableUser()
//{
//
//    tempBuffer[0] = NULL;
//    buffer[0] = NULL;
//    strcat(buffer, "[");
//    String UserCNP = server.arg("CNP");
//    sprintf(tempBuffer, "UPDATE Users SET isActive = '0' WHERE CNP = %s; ", UserCNP.c_str());
//    Serial.print("\nbuffer: ");
//    Serial.print(tempBuffer);
//    const  char* userQuery = tempBuffer;
//    rc = db_exec(db1, userQuery);
//    if (rc != SQLITE_OK)
//    {
//        strcat(buffer, "]");
//        server.send(200, "application/json", buffer);
//        return;
//    }
//    int len = strlen(buffer);
//    if (buffer[len - 2] == ',')
//    {
//        buffer[len - 2] = ']';
//    }
//    else
//    {
//        strcat(buffer, "]");
//    }
//    server.send(200, "application/json", buffer);
//
//}
//
//void EditUser() {
//    tempBuffer[0] = NULL;
//    buffer[0] = NULL;
//    strcat(buffer, "[");
//    String UserCNP = server.arg("CNP");
//    String NewFirstName = server.arg("toEditFirstName");
//    String NewLastName = server.arg("toEditLastName");
//    String NewPassword = server.arg("toEditPassword");
//    String NewCredit = server.arg("toEditCredit");
//    String NewIsAdmin = server.arg("toEditAdmin");
//    String NewTagNumber = server.arg("toEditTagNumber");
//    sprintf(tempBuffer, "UPDATE Users SET FirstName = '%s', LastName = '%s', Password = '%s', Credit = '%s', IsAdmin = '%s', TagNumber = '%s' WHERE CNP = '%s'; ", NewFirstName.c_str(), NewLastName.c_str(), NewPassword.c_str(), NewCredit.c_str(), NewIsAdmin.c_str(), NewTagNumber.c_str(), UserCNP.c_str());
//    Serial.print("\nbuffer: ");
//    Serial.print(tempBuffer);
//    const  char* EditUserQuery = tempBuffer;
//    rc = db_exec(db1, EditUserQuery);
//    if (rc != SQLITE_OK)
//    {
//        strcat(buffer, "]");
//        server.send(605, "application/json", buffer);
//        return;
//    }
//    else
//    {
//        int len = strlen(buffer);
//        if (buffer[len - 2] == ',')
//        {
//            buffer[len - 2] = ']';
//        }
//        else
//        {
//            strcat(buffer, "]");
//        }
//        server.send(200, "application/json", buffer);
//        
//    }
//
//}
//
//void AddUser() {
//    tempBuffer[0] = NULL;
//    buffer[0] = NULL;
//    strcat(buffer, "[");
//
//    String UserCNP = server.arg("AddCNP");
//    String FirstName = server.arg("AddFirstName");
//    String LastName = server.arg("AddLastName");
//    String Password = server.arg("AddPassword");
//    String IsAdmin = server.arg("CheckIfAdmin");
//    String TagNumber = server.arg("AddTagNumber");
//
//    sprintf(tempBuffer, "INSERT INTO Users(CNP, FirstName, LastName, Password, Credit, TagNumber, IsAdmin, IsActive) VALUES(%s, '%s', '%s', '%s', 0, '%s', %s, 1);", UserCNP.c_str(), FirstName.c_str(), LastName.c_str(), Password.c_str(), TagNumber.c_str(), IsAdmin.c_str());
//    Serial.print("\nbuffer: ");
//    Serial.print(tempBuffer);
//    const  char* EditUserQuery = tempBuffer;
//    rc = db_exec(db1, EditUserQuery);
//    if (rc != SQLITE_OK)
//    {
//        strcat(buffer, "]");
//        server.send(605, "application/json", buffer);
//        return;
//    }
//    else
//    {
//        int len = strlen(buffer);
//        if (buffer[len - 2] == ',')
//        {
//            buffer[len - 2] = ']';
//        }
//        else
//        {
//            strcat(buffer, "]");
//        }
//        server.send(200, "application/json", buffer);
//
//    }
//}

const char* const jsonUser = "{\"CNP\":%s, \"FirstName\": \"%s\", \"LastName\":\"%s\", \"isAdmin\": %s, \"Credit\": %s,\"TagNumber\": \"%s\", \"isActive\": %s}";

const char* const data = "Callback function called";
static int callback(void* data, int argc, char** argv, char** azColName)
{

    strcat(buffer, "{");
    tempBuffer[0] = NULL;
    for (i = 0; i < argc; i++)
    {
        if (i == argc - 1)
        {
            sniprintf(tempBuffer, 50, "\"%s\": \"%s\"", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        else
        {
            sniprintf(tempBuffer, 50, "\"%s\": \"%s\", ", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        strcat(buffer, tempBuffer);
    }
    strcat(buffer, "}, ");

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
    Serial.print("\nPrinting SQL: ");
    Serial.print(sql);
    Serial.print("\n");


    Serial.println("\ndb_exec");
    int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        Serial.printf("SQL error: %s\n", zErrMsg);
        for (int i = 0; i < strlen(zErrMsg); i++)
        {
            if (zErrMsg[i] == '\"')
            {
                zErrMsg[i] = '\'';
            }
        }
        strcat(buffer, "{ \"ErrorText\": \"");
        strcat(buffer, zErrMsg);
        strcat(buffer, "\"}");
        sqlite3_free(zErrMsg);
    }
    else
    {
        Serial.printf("Operation done successfully\n");
    }
    return rc;
}

void setup()
{
    system_update_cpu_freq(SYS_CPU_160MHZ);

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
    //SPIFFS.remove("/test1.db");
    SPIFFS.remove("/ico.ico");
    SPIFFS.remove("/VendingMachineDB.db");

    sqlite3_initialize();

    // Open databases
    vfs_set_spiffs_file_obj(&db_file_obj_1);
    if (db_open("/FLASH/vm.db", &db1))
        return;
        
         //Create table Transactions
    rc = db_exec(db1, "SELECT * FROM User;");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

/*
            //Create table User
    rc = db_exec(db1, "CREATE TABLE Users (CNP INTEGER PRIMARY KEY, FirstName VARCHAR2(16), LastName VARCHAR2(16), Password VARCHAR2(12) NOT NULL, TagNumber VARCHAR2(16), Credit NUMBER(4,2) NOT NULL, IsAdmin NUMBER(1) NOT NULL, IsActive NUMBER(1) NOT NULL);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

 //Create table Transactions
    rc = db_exec(db1, "CREATE TABLE Action(ID INTEGER PRIMARY KEY, Timestamp TIMESTAMP NOT NULL, TypeID NUMBER(1) NOT NULL, Credit NUMBER(4,2), Quantity NUMBER(2));");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

    //Create table Products
    rc = db_exec(db1, "CREATE TABLE Product (ID INTEGER PRIMARY KEY, Name VARCHAR2(30),Price NUMBER(2,1), Position NUMBER(1), isActive NUMBER(1) NOT NULL, Quantity NUMBER(2) NOT NULL);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

    //Insert user into table
    rc = db_exec(db1, "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES (19712073264,'Ion', 'Vasilescu', 'abc', 4.85, 1, 1);");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }

    //Insert user into table
    rc = db_exec(db1, "INSERT INTO Users (CNP, FirstName, LastName, Password, Credit, isAdmin, isActive) VALUES  (1, 'Anonymus', 'Anonymus', '1', 0, 0, 1 );");
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db1);
        return;
    }
   */
    //m3.Rotate(false, 200);
   
    //for removing files use he commend bellow
    //SPIFFS.remove("/index.htm");

    server.on("/", GetIndexPage);
    server.on("/LogIn", LogIn);
    //server.on("/Motor1", Motor1);
    //server.on("/Motor2", Motor2);
    //server.on("/Motor3", Motor3);
    server.on("/RunCommand", RunCommand);
    //server.on("/Users", Users);
    //server.on("/EnableUser", EnableUser);
    //server.on("/DisableUser", DisableUser);
    //server.on("/EditUser", EditUser);
    //server.on("/AddUser", AddUser);
    //server.on("/Products", Products);
    server.begin();

}
void loop()
{
    server.handleClient();
}
