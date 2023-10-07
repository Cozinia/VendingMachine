// 
// 
// 

#include "FileDb.h"
FileDb fileDb;

FileDb::FileDb()
{
	this->init();
}

void FileDb::init()
{
	Serial.print("init()");
	this->userCount = 0;
	this->productCount = 0;
	this->transactionCount = 0;

}

void FileDb::InitData()
{
	
	User user1;
	strcpy(user1.CNP, "19712073264");
	strcpy(user1.Password, "abc");
	strcpy(user1.FirstName, "Ion");
	strcpy(user1.LastName, "Vasilescu");
	strcpy(user1.TagNumber, "89178228185");
	user1.isAdmin = 1;
	user1.Enabled = 1;
	user1.Credit = 2.40;
	AddUser(&user1);

	strcpy(user1.CNP, "24512103804");
	strcpy(user1.Password, "1234");
	strcpy(user1.FirstName, "Maria");
	strcpy(user1.LastName, "Popescu");
	strcpy(user1.TagNumber, "22918314235");
	user1.isAdmin = 0;
	user1.Enabled = 1;
	user1.Credit = 3.10;
	AddUser(&user1);
	
	Product product1;

	product1.ID = 1;
	strcpy(product1.Name, "Snikers");
	product1.Position = 3;
	product1.Price = 1.5;
	product1.Enabled = 1;
	product1.Quantity = 5;
	AddProduct(&product1);

	product1.ID = 2;
	strcpy(product1.Name, "Mars");
	product1.Position = 1;
	product1.Price = 2.00;
	product1.Enabled = 1;
	product1.Quantity = 4;
	AddProduct(&product1);

	product1.ID = 3;
	strcpy(product1.Name, "Twix");
	product1.Position = 1;
	product1.Price = 2.5;
	product1.Enabled = 1;
	product1.Quantity = 4;
	AddProduct(&product1);

	product1.ID = 4;
	strcpy(product1.Name, "7Days");
	product1.Position = 1;
	product1.Price = 1.3;
	product1.Enabled = 1;
	product1.Quantity = 2;
	AddProduct(&product1);

	//LoadAll();
	/*
	

	Product product1;
	product1.ID = 1;
	strcpy(product1.Name, "Snikers");
	product1.Position = 3;
	product1.Price = 1.5;
	product1.Enabled = 1;
	product1.Quantity = 5;
	AddProduct(&product1);

	
	/*
	
	User user1;
	strcpy(user1.CNP, "19712073264");
	strcpy(user1.Password, "abc");
	strcpy(user1.FirstName, "Ion");
	strcpy(user1.LastName, "Vasilescu");
	strcpy(user1.TagNumber, "89178228185");
	user1.isAdmin = 1;
	user1.Enabled = 1;
	user1.Credit = 2.40;
	AddUser(&user1);

	Product product1;
	product1.ID = 1;
	strcpy(product1.Name, "Snikers");
	product1.Position = 3;
	product1.Price = 1.5;
	product1.Enabled = 1;
	product1.Quantity = 5;
	AddProduct(&product1);

	product1.ID = 2;
	strcpy(product1.Name, "Mars");
	product1.Position = 1;
	product1.Price = 2.00;
	product1.Enabled = 1;
	product1.Quantity = 4;
	AddProduct(&product1);

	product1.ID = 3;
	strcpy(product1.Name, "Twix");
	product1.Position = 1;
	product1.Price = 2.5;
	product1.Enabled = 1;
	product1.Quantity = 4;
	AddProduct(&product1);

	product1.ID = 4;
	strcpy(product1.Name, "7Days");
	product1.Position = 1;
	product1.Price = 1.3;
	product1.Enabled = 1;
	product1.Quantity = 2;
	AddProduct(&product1);

	
	/*
	strcpy(user1.CNP, "24512103804");
	strcpy(user1.Password, "1234");
	strcpy(user1.FirstName, "Maria");
	strcpy(user1.LastName, "Popescu");
	strcpy(user1.TagNumber, "22918314235");
	user1.isAdmin = 0;
	user1.Enabled = 1;
	user1.Credit = 3.10;
	AddUser(&user1);

	Product product1;
	product1.ID = 1;
	strcpy(product1.Name, "Snikers");
	product1.Position = 3;
	product1.Price = 1.5;
	product1.Enabled = 1;
	product1.Quantity = 5;
	AddProduct(&product1);

	product1.ID = 2;
	strcpy(product1.Name, "Mars");
	product1.Position = 1;
	product1.Price = 2.00;
	product1.Enabled = 1;
	product1.Quantity = 4;
	AddProduct(&product1);

	product1.ID = 3;
	strcpy(product1.Name, "Mars");
	product1.Position = 1;
	product1.Price = 2.5;
	product1.Enabled = 1;
	product1.Quantity = 4;
	AddProduct(&product1);

	product1.ID = 4;
	strcpy(product1.Name, "Mars");
	product1.Position = 1;
	product1.Price = 1.3;
	product1.Enabled = 1;
	product1.Quantity = 2;
	AddProduct(&product1);
	*/
}

int FileDb::AddUser(User* entity)
{
	bool exists = false;
	for (int i = 0; i < this->userCount; i++)
	{
		if (strcmp(entity->CNP, this->users[i].CNP)==0)
		{
			exists = true;
			return 1;
		}
	}

	this->users[this->userCount] = *entity;
	this->userCount++;
	//SaveAll();
	return 0;
}

User* FileDb::GetUserByCNP(char* cnp)
{
	
	for (int i = 0; i < this->userCount; i++) {
		if (strcmp(cnp, this->users[i].CNP)==0) {
			
			return &this->users[i];
		}
	}
	return NULL;
}


int FileDb::AddProduct(Product* entity) {
	bool exists = false;
	for (int i = 0; i < this->productCount; i++) {
		if (entity->ID == this->products[i].ID) {
			exists = true;
			return 1;
		}
	}
	this->products[this->productCount] = *entity;
	this->productCount++;
	//SaveAll();
	return 0;
}

int FileDb::AddTransaction(Transaction* entity) {
	bool exists = false;
	for (int i = 0; i < this->transactionCount; i++) {
		if (entity->ID == this->transactions[i].ID) {
			exists = true;
			return 1;
		}
	}
	this->transactions[this->transactionCount] = *entity;
	this->transactionCount++;
	return 0;
}

int FileDb::UpdateUser(User* entity) {
	for (int i = 0; i < this->userCount; i++) {
		if (strcmp(entity->CNP, this->users[i].CNP) == 0) {
			this->users[i] = *entity;
			return 0;
		}
		
	}

	return 1;
}

int FileDb::UpdateProduct(Product* entity) {
	for (int i = 0; i < this->productCount; i++) {
		if (entity->ID == this->products[i].ID) {
			this->products[i] = *entity;
			return 0;
		}
	}
	return 1;

}
/*
void FileDb::SaveAll()
{
		File myFile = SPIFFS.open("/FileDb.all", "w");
		myFile.write((byte*)this, sizeof(FileDb));
		myFile.close();
}

void FileDb::LoadAll() {
	

	File myFile = SPIFFS.open("/FileDb.all", "r");
	if (myFile)
	{
		myFile.read((byte*)this, sizeof(FileDb));
		myFile.close();
	}

}
*/


