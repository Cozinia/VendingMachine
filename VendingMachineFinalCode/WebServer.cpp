// 
// 
// 

#include "WebServer.h"
#include "FileDb.h"
#include <string>
#include "ActionOnMotors.h"

ESP8266WebServer server(80);


void WebServer__GetIndexPage()
{
	File file = SPIFFS.open("/index.html", "r");
	server.streamFile(file, "text/html");
}

int LogIn()
{
	String cnp = server.arg("Username");
	String pwd = server.arg("Password");	
	User* user = fileDb.GetUserByCNP((char*)cnp.c_str());
	if (user != NULL)
	{
		if (strcmp(user->Password, pwd.c_str()) == 0)
		{
			sprintf(MyBuffer, "[{\"CNP\":\"%s\", \"FirstName\": \"%s\", \"LastName\":\"%s\", \"isAdmin\": %d, \"Credit\": %f,\"TagNumber\": %ld, \"isActive\": %d}]", user->CNP, user->FirstName, user->LastName, user->isAdmin, user->Credit, user->TagNumber, user->Enabled);
			server.send(200, "application/json", MyBuffer);
		}
		else
		{
			server.send(601, "text/plain", "Invalid CNP or password");
		}
	}
	server.send(602, "text/plain", "User doesn't exist!");
	return 1;
}

void Users() {
	MyBuffer[0] = NULL;
	TempBuffer[0] = NULL;
	strcat(MyBuffer, "[");
	for (int i = 0; i < fileDb.userCount; i++)
	{
		Serial.print(MyBuffer);
		sprintf(TempBuffer, "{\"CNP\":\"%s\", \"FirstName\": \"%s\", \"LastName\":\"%s\", \"isAdmin\": %d, \"Credit\": %f,\"TagNumber\": \"%s\", \"isActive\": %d}", fileDb.users[i].CNP, fileDb.users[i].FirstName, fileDb.users[i].LastName, fileDb.users[i].isAdmin, fileDb.users[i].Credit, fileDb.users[i].TagNumber, fileDb.users[i].Enabled);
		if (i < fileDb.userCount - 1)
		{
			strcat(MyBuffer, TempBuffer);
			strcat(MyBuffer, ",");
		}
		else
		{
			strcat(MyBuffer, TempBuffer);
		}
	}
	strcat(MyBuffer, "]");
	server.send(200, "application/json", MyBuffer);
}

void Products() {
	MyBuffer[0] = NULL;
	TempBuffer[0] = NULL;
	strcat(MyBuffer, "[");
	for (int i = 0; i < fileDb.productCount; i++)
	{
		Serial.print(MyBuffer);
		sprintf(TempBuffer, "{\"ID\": %d, \"Name\": \"%s\", \"Position\":%d, \"Price\": %f, \"Quantity\": %d,\"isActive\": %d}", fileDb.products[i].ID, fileDb.products[i].Name, fileDb.products[i].Position, fileDb.products[i].Price, fileDb.products[i].Quantity, fileDb.products[i].Enabled);
		if (i < fileDb.productCount - 1)
		{
			strcat(MyBuffer, TempBuffer);
			strcat(MyBuffer, ",");
		}
		else
		{
			strcat(MyBuffer, TempBuffer);
		}
	}
	strcat(MyBuffer, "]");
	server.send(200, "application/json", MyBuffer);
}

void AddUser() {
	String cnp = server.arg("AddCNP");
	String firstName = server.arg("AddFirstName");
	String lastName = server.arg("AddLastName");
	String password = server.arg("AddPassword");
	String tag = server.arg("AddTagNumber");
	String admin = server.arg("CheckIfAdmin");
	User user;
	strcpy(user.CNP, cnp.c_str());
	strcpy(user.FirstName, firstName.c_str());
	strcpy(user.LastName, lastName.c_str());
	strcpy(user.Password, password.c_str());
	strcpy(user.TagNumber, tag.c_str());
	user.isAdmin = admin.toInt();
	fileDb.AddUser(&user);
	server.send(200, "text/plain", "ok");

}

void AddProduct()
{
	String id = server.arg("AddProductID");
	String name = server.arg("AddProductName");
	String quantity = server.arg("AddProductQuantity");
	String positon = server.arg("AddProductPosition");
	String price = server.arg("AddProductPrice");
	Product product;
	product.ID = id.toInt();
	strcpy(product.Name, name.c_str());
	product.Quantity = quantity.toInt();
	product.Position = positon.toInt();
	product.Position = positon.toInt();

	fileDb.AddProduct(&product);
	server.send(200, "text/plain", "ok");

}

void EditUsers() {
	String cnp = server.arg("CNP");
	String firstName = server.arg("toEditFirstName");
	String lastName = server.arg("toEditLastName");
	String password = server.arg("toEditPassword");
	String credit = server.arg("toEditCredit");
	String admin = server.arg("toEditAdmin");
	String tagNumber = server.arg("toEditTagNumber");

	Serial.println(cnp.toInt());
	User user;
	strcpy(user.CNP, cnp.c_str());
	strcpy(user.FirstName, firstName.c_str());
	strcpy(user.LastName, lastName.c_str());
	strcpy(user.Password, password.c_str());
	user.Credit = credit.toInt();
	user.isAdmin = admin.toInt();
	strcpy(user.TagNumber, tagNumber.c_str());
	if (fileDb.UpdateUser(&user) == 0)
	{
		server.send(200);
	}
	else
	{
		server.send(604);
	}
}

void EditProduct() {
	String id = server.arg("ID");
	String name = server.arg("ProductName");
	String position = server.arg("ProductPosition");
	String price = server.arg("ProductPice");
	String quantity = server.arg("ProductQuantity");
	Serial.println(" ");
	Serial.println(id.toInt());
	Product product;
	product.ID = id.toInt();
	strcpy(product.Name, name.c_str());
	product.Position = position.toInt();
	product.Price = price.toInt();
	product.Quantity = quantity.toInt();

	if (fileDb.UpdateProduct(&product) == 0) 
	{
		server.send(200);
	}
	else {
		server.send(604);
	}
}

void WebServer__Init()
{
	
	server.on("/", WebServer__GetIndexPage);
	server.on("/LogIn", LogIn);
	server.on("/Users", Users);
	server.on("/Products", Products);
	server.on("/AddUser", AddUser);
	server.on("/AddProduct", AddProduct);
	server.on("/EditProduct", EditProduct);
	server.on("/EditUsers", EditUsers);
	server.on("/Motor1", Motor1);
	server.on("/Motor2", Motor2);
	server.on("/Motor3", Motor3);
	server.on("/Motor4", Motor4);
	server.on("/OpenDoor", OpenDoor);
	server.begin();
}




void WebServer__Process() 
{
	server.handleClient();
}



