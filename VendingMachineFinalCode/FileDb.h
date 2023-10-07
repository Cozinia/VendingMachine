// FileDb.h

#ifndef _FileDb_h
#define _FileDb_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <FS.h>
#include <SPI.h>

typedef struct
{
	int size;
	int count;
	int free;
}TableHeader;


typedef struct
{
	char CNP[12];
	char FirstName[20];
	char LastName[15];
	char Password[20];
	char TagNumber[15];
	float Credit;
	int isAdmin;
	int Enabled;
}User;

typedef struct 
{
	int ID;
	char Name[20];
	float Price;
	int Position;
	bool Enabled;
	int Quantity;
}Product;


typedef struct
{
	int ID;
	long int Timestamp;
	int Type;
	float Credit;
	int Quantity;
	char CNP[11];
	int ProductID;

}Transaction;


#define UserMaxRecords 15
#define ProductMaxRecords 10
#define TransactionMaxRecords 100

class FileDb
{
 protected:


 public:
	 int userCount;
	 int productCount;
	 int transactionCount;
	 User users[UserMaxRecords];
	 Product products[ProductMaxRecords];
	 Transaction transactions[TransactionMaxRecords];



 public:
	 FileDb();
	void init();
	void InitData();
	int AddUser(User*);
	User* GetUserByCNP(char* cnp);
	int AddProduct(Product*);
	int AddTransaction(Transaction*);
	int UpdateUser(User*);
	int UpdateProduct(Product*);
	void SaveAll();
	void LoadAll();
	void Process();
};

extern FileDb fileDb;

#endif

