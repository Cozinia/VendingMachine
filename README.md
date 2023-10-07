
# Vending Machine

![VendingMachinePhoto](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/Vending%20Machine%202022-05-19%20at%2012.28.13%20PM.jpeg)


## Descrierea componentelor folosite

În cadrul acestui proiect voi fi folosite mai multe componente așa că mai jos se află  o mică prezentare a acestora și cum sunt legate la microcontroler.
### Numărătorul de monede
Primul pe ordinea de idei este numărătorul de monede, `CH-926` pe numele de cod, acesta acceptă până la 3 tipuri diferite de monede. 
Acest modul trebuie conectat la o sursă de 12V , iar pinul ce se ocupă cu transmiterea mesajului de identificare a monedelor trebuie conectat la unul dintre pinii digitali ai microcontroler-ului ca în schema de mai jos.

![CoinAcceptorWithDisplay](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Vending%20machine%20Fritzing/CoinAcceptorWithDisplay.png)

În desen se observă microcontroler-ul în partea stângă, sursa de 12V în partea de sus și numărătorul de monede în partea dreaptă.Acesta este conectat la sursa externă de 12V prin cele două fire(roșu pentru VCC și albastru pentru GND), iar prin firul de culoare galbenă la `ESP8266`, mai exact la 3V3 ce unește printr-un tranzistor pinul SIG(Coin) al acceptorului de monede cu pinul digital D3 al plăcuței.

Modulul este progrmat pentru a distinge 3 monede diferite(de `5, 10 și 50 bani`).Acesta se bazează pe pulsuri consecutive pentru fiecare tip de monedă în parte(un puls pentru 5 bani, două pulsuri pentru 10 bani, trei pulsuri pentru 50 de bani).

Iată câteva secvențe de cod interesante:

```
attachInterrupt(digitalPinToIntrerrupt(CoinPin), CoinEvent, CHANGE);
```
 
Pentru a nu supraîncărca plăcuța cu citiri successive se folosește attachInterrupt, metodă cu ajutorul căreia funcția CoinEvent este invocată doar la detectarea introducerii unei monede.
  
```
void CoinEvent()
{
    crtValue = digitalRead(CountPin);

    Serial.print("CoinEVENT_____________");
    Serial.print("\n");
    Serial.print(crtValue);
    Serial.print("\n");

}

```

`digitalRead` citește valoarea pinului Count și o stochează în variabila `crtValue`.

Funcția `Coins` se apelează în loop. Dacă timpul curent este diferit de cel anterior iar valoarea curentă de cea anterioară, timpul anterior ia valoarea celui prezent și valoarea anterioară ia valoarea celei prezente. Variabila de tip bool  numită changed își schimbă valoarea în true când această condiție este îndeplinită.Dacă diferența dintre timpul curent  și cel anterior este mai mare de 30 de milisecunde, iar changed este true, variabila number crește cu o unitate.


```
void Coins()
{
    //take current time
    crtTime = millis();
    //compare current time with previous time
    if (crtTime > prevTime)
    {
        //compare current value with previous value
        if (crtValue != prevValue)
        {
            prevTime = crtTime;

            prevValue = crtValue;
            changed = true;
        }
        if (crtTime - prevTime > 30 && changed)
        {
            counter++;
            Serial.print(counter);
            Serial.print("\n");
            changed = false;
            number++;
            Time1 = millis();
            changed = false;
        }
    }
    Time2 = millis();
    if ((Time2 - Time1 > TIMER) && (number))
    {

        Serial.print("Number: ");
        Serial.print(number);
        Serial.print("\n");
        Serial.print("Time2-Time1: ");
        Serial.print(Time2 - Time1);
        Serial.print("\n");

        if (number == 3)
        {
            credit = credit + 0.50;
            totalCoins3++;
            number = 0;
        }
        else if (number == 2)
        {
            credit = credit + 0.10;
            totalCoins2++;
            number = 0;
        }
        else
            if (number == 1)
            {
                totalCoins1++;
                credit = credit + 0.05;
                number = 0;
            }
        lcd.setCursor(0, 0);
        lcd.print("Credit: ");
        lcd.setCursor(8, 0);
        lcd.print(credit);
        Serial.print("Credit: ");
        Serial.print(credit);
        Serial.print("\n");
    }
}

```


În funcție de valoarea variabilei number, aceasta îndeplinește una dintre condițiile de mai sus.În acest fel creditul crește cu o anumită valoare(5, 10 sau 50 de bani), valoarea number este resetată la 0, iar variabila totalCoins crește cu o unitate pentru a înregistra numărul total de monede de un anumit tip.

### Display-ul

După punerea în funcțiune a numărătorului de monede a fost adăugat și un display pe care să afișeze creditul inserat.Mai jos este inserată o schemă ce conține și display-ul.

![CoinAcceptorWithDisplay](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Vending%20machine%20Fritzing/CoinAcceptorWithDisplay.png)

Display-ul comunică prin protocolul I2C cu microcontroler-ul. Pentru a face acest lucru posibil se conectează pinii D1 la SCL(reprezentat prin cablul de culoare mov), D2 la SDA(reprezentat prin cablul de culoare oranj), pinul VIN(cel care transportă curent de 5V) la VCC și GND la GND.

```
//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1

#include <Wire.h>

#include <LiquidCrystal I2C.h>

LiquidCrystal I2C lcd(@x27, 16, 2); // set the LCD address to @x27 for a 16 chars and 2 line display

void setup()
{
lcd.init(); // initialize the lcd
i // Print a message to the LCD's screen
 lcd.backlight();

lcd.setCursor(@, 0);
lcd.print(“Atestat 2022 by");
lcd.setCursor(@, 1);
lcd.print(“Cozinia Motronu™);

}

void loop()
{

}


```
 
Acest cod folosește librăria „`LiquidCrystal_I2C`”.După declararea directivelor preprocesor „`Wire`” și „`LiquidCrystal_I2C`” se inițializează obiectul lcd care se află la adresa 0x27.În funcția setup se inițializează `lcd`, `backlight` și se setează cursorul pe poziția linia 0 și coloana 0 pentru a scrie un mesaj, respectiv lini 1 și coloana 0 pentru continuarea mesajului.
Mai jos este inserată o poză cu  un mesaj afișat pe display.

![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/DisplayLED.jpg)


### Motoarele

Acum schema începe să se încarce deoarece fiecare motor are 4 fire cu ajutorul cărora se conectează la shield-ul `CNC` și alte două care duc la modulul de expansiune `PCF8574`. Se folosesc aceste module deoarece atât motoarele, cât și celelalte componente ocupă foarte mulți pini și nu lasă loc disponibil pe microcontroler. `PCF8574` folosește același protocol precum display-ul, adică `I2C` pentru a transmite diferite evenimente ce se petrec pe pini.Acest lucru este convenabil deoarece se pot lega mai multe module de expansiune pentru a extinde numărul de pini.Aceste module se diferențiază prin adresă, fiecare putând fi programat manual ca având o anumită adresă(de la `0x20 la 0x27`) așa cum este ilustrat în imaginea de mai jos.



![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Imagini%20componente/PCF8574_adresses.png)


	

	
Shield-ul CNC împreună cu driver-ul de motoare `A4988` asigură buna funcționare a motoarelor oferind o interfață ușor de utilizat. 
Shield-ul oferă suport pentru maxim 4 drivere de motoare, un `pinEnable(EN)` pentru a activa mișcarea motoarelor, mai mulți pini de `ground(GND)`, pini care permit mișcarea motoarelor într-o anumită direcție și un anumit număr de pași, de exemplu: `X.STEP`(indică numărul de pași pe care motorul X să îl parcurgă), `X.DIR`(direcția în care să se învârtă motorul X). Shield-ul trebuie alimentat cu `12V` de la o sursă externă și 5V(curent necesar cerut de driver-ele de motoare).
	
![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/CNC%20Shield%20with%20A4988.jpg)


Mai jos se poate observa schema proiectului la care se adaugăt motoarele, shield-ul cu driver-ele de motoare și modulele de expansiune a pinilor.



![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Vending%20machine%20Fritzing/CoinAcceptorWithDisplayAndMotors_bb.png)


### Modulul RFID

`RFID` este acronimul de la "radio-frequency identification" (identificare prin radiofrecvență) și se referă la o tehnologie prin care datele digitale codificate în etichete RFID sau etichete inteligente sunt captate de un cititor prin intermediul undelor radio. Tehnologia RFID este similară codului de bare, în sensul că datele de pe o etichetă sunt capturate de un dispozitiv care stochează datele într-o bază de date. Cu toate acestea, RFID are mai multe avantaje față de sistemele care utilizează un software de urmărire a activelor cu coduri de bare. Cel mai notabil este faptul că datele din etichetele RFID pot fi citite în afara liniei de vizibilitate, în timp ce codurile de bare trebuie aliniate cu un scaner optic.
Modul de utilizare este destul de simplu: utilizatorul  apropie cardul de scanner și datele sunt citite și prelucrate.În cazul acestui proiect utilizatorul folosește cardul pentru a se înregistra la vendomat, nemaifiind nevoie să introducă bani pentru a achiziționa produse.
Modulul RFID este conectat ca în schema de mai jos la pinii următori:

![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Imagini%20componente/RFID_ESP8266_Pins.png)


![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Vending%20machine%20Fritzing/ESP8266_RC522.png)



Pentru a putea folosi modulul RFID trebuie instalată librăria `MFRC522`.

Codul folosit este descris mai jos:
 
```
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D3 // Configurable, see typical pin layout above
#define SS_PIN D8 // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup()
{
Serial.begin(96060); // Initialize serial communications with the PC
SPI.begin(); // Init SPI bus
mfrc522.PCD_Init(); // Init MFRC522
mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{
// Look for new cards
if (!mfrc522.PICC_IsNewCardPresent())
{
	return;
}
// Select one of the cards

if (!mfrc522.PICC_ReadCardSerial())
{
	return;
}
// Dump debug info about the card; PICC_HaltA() is automatically called

mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

return;
}


```


După încărcarea acestui cod pe serială ar trebui să apară un mesaj asemănător cu cel de mai jos(mesajul poate să difere în funcție de versiunea moduluului RFID) unde puteți scana cardul.


![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/SerialaRFIDRead.png)


Mai departe în implementarea codului care va identifica ID-ul cardului se va folosi „Card UID” .




### Stocarea datelor

Din cauza limitărilor spațiului de stocare pe care le au microcontroler-ele `ESP8266`(8MB RAM), nu este posibilă realizarea unei baze de date care să stocheze utilizatorii, produsele și tranzacțiile și în același timp să execute și comenzile celorlalte componente. În acest caz se folosesc structuri care imită tabelele enumerate mai sus.A cestea sunt realizate într-o clasă numită `FileDb`.


```
typedef struct
{
	char CNP[14];
	char FirstName[20];
	char Lastname[15];
	char Password[20];
	char TagNumber[15];
	float Credit;
	int isAdmin;
	int Enabled;

}User;
```


Această structură reține CNP-ul, nume, prenumele, parola, numărul cardului `RFID`, creditul, statutul utilizatorului(dacă este sau nu administrator) , iar parametrul Enabled are valoarea 1 dacă acesta este activ și 0 în caz contrar. Celelalte structuri arată similar.
După cum îi spune și numele, funcția „`AddUser()`” adaugă utilizatori. Aceasta primește ca parametru o entitate de tip User* și verifică dacă CNP-ul utilizatorului care se dorește să fie introdus se află deja în structură. Dacă da, funcția returnează 1, iar în caz contrar stochează datele noului utilizator și incrementează `userCount`. Procesul este similar pentru produse și tranzacții.


```
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
```

În funcția `InitData` sunt creați câțiva utilizatori, produse și tanzacții urmând modelul din figurile inserate mai jos.

```
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


	Product product1;

	product1.ID = 1;
	strcpy(product1.Name, "Snikers");
	product1.Position = 3;
	product1.Price = 1.5;
	product1.Enabled = 1;
	product1.Quantity = 5;
	AddProduct(&product1);

}
```

### Aplicația web
	
Aplicația web este construită folosind `HTML`, `CSS` și `VueJS`. Din cauza spațiului mic de stocare al microcontroler-ului această aplicație web este construită pe o singură pagină.
Primul element care va apărea în pagină este formularul de logare.În acest formular utilizatorul va introduce CNP-ul și parola.Prin apăsare butonului `Login` se va apela funcția `LogIn()`.

```
 <div id="app" v-model="StartUp()" v-cloak>
        <!--Login page-->
        <div id="Menu2" v-if="!UserIsLogged">
            <div style=" margin: 150px 0px 0px 0px; text-align: center;">
                <h1 style="text-align:center;">Login form</h1>
                <form class="ui form">
                    <div class="field">
                        <label>Username</label>
                        <input type="text" id="username" placeholder="Username">
                    </div>
                    <div class="field">
                        <label>Password</label>
                        <input type="text" id="password" placeholder="Password">
                    </div>
                    <button class="ui button" type="button" v-on:click="LogIn()"><p>Login</p></button>
                </form>
            </div>
        </div>

```

```
 async LogIn() {
            var username = document.getElementById("username").value;
            var password = document.getElementById("password").value;      
            var url = encodeURI("/LogIn" + "?" + "Username=" + username + "&" + "Password=" + password);
            var result = await fetch(url);   
            if (result.status == 200) {
                this.UserInfo.Data = await result.json();
                if (!this.UserInfo.Data[0].isAdmin) {
                    this.UserIsLogged = true;
                    this.isAdmin = true;
                    alert("Welcome back, " + this.UserInfo.Data[0].FirstName + "!");  
                } else if (this.UserInfo.Data[0].isAdmin) {
                    this.UserIsLogged = true;
                    this.isAdmin = false;
                    alert("Welcome back, " + this.UserInfo.Data[0].FirstName + "!");
                } 
            }else {
                this.UserInfo.Error.Message = await result.json();
                alert(this.UserInfo.Error.Message);
                this.UserIsLogged = false;
                this.isAdmin = false;
            }                 
        }
```
 
 
Această funcție este de tip `async` deoarece așteaptă rezultatul de la server. Datele introduse de utilizator sunt aduse aici prin instrucțiunea `document.getElementById`. Acestea sunt trimise la server și interpretate.

```
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
```


Această funcție se află pe server și interpretează datele introduse. Acestea sunt preluate din aplicația web prin instrucțiunea `server.arg(„ExNume”)`.Variabila user de tip `User*` stochează răspunsul funcției `GetElementByCNP` aflată în clasa `FileDb`.


```
User* FileDb::GetUserByCNP(char* cnp)
{
	
	for (int i = 0; i < this->userCount; i++) {
		if (strcmp(cnp, this->users[i].CNP)==0) {
			
			return &this->users[i];
		}
	}
	return NULL;
}
```

Această funcție verifică dacă parametrul `cnp`  corespunde utilizatorului `users[i]`. În caz afirmativ returnează datele utilizatorului, iar în caz contrar `NULL`.

În funcția `LogIn` se verifică dacă parola utilizatului primit anterior corespunde cu cea trimisă din aplicația web. În caz afirmativ datele se formatează în format `json` folosind funcția `sprintf` și se trimit în pagina web ca răspuns alături de codul 200(ok). În caz contrar se trimite codul de eroare 601(acest cod este creat de mine și înseamnă „CNP sau parolă invalidă”).În cazul în care nici parola și nici CNP-ul nu se potrivesc cu datele existente deja se trimite codul de eroare 602(utilizatorul nu există).

Dacă CNP-ul și parola se potrivesc, pe client se primește codul 200(ok) și se verifică dacă utilizatorul este administrator sau nu.În acest afirmativ, variabila de tip bolean isAdmin ia valoarea true, în caz contrar ia valoarea false. În ambele dintre aceste cazuri este afișat mesajul „Welcome back” urmat de numele utilizatorului.În caz că datele introduse nu se potrivesc celor existente în structură se va afișa mesajul venit de la server stocat în `UserInfo.Error.Message`.

După ce utilizatorul a fost logat variabila de tip bolean `UserIsLogged`  ia valoarea true, iar în funcție de valoarea variabilei isAdmin se afișează meniul administratorului sau al utilizatorului.


### Meniul administratorului
Mai jos este inserată  o captură de ecran reprezentând structura paginii administratorului.Acesta poate vedea lista utilizatorilor și a produselor, poate adăuga utilizatori și produse, poate deschide ușa vendomatului și nu în ultimul rând, poate testa funcționalitatea motoarelor.


![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/MainPageAdmin.png)






Meniul de vizualizare a utilizatorilor afișează toți utilizatorii și informațiile acestora.În acest meniu administratorul poate edita datele acestora și îi poate dezactiva.
	
![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/UsersMenu.png)

Pentru a face acest lucru posibil, la apăsarea butonului `ViewUsers` se apelează funcția `GetUsers`.


```
 async GetUsers() {
	const responseUsers = await fetch("/Users");
	this.UserInfo.Users = await responseUsers.json();
	this.users = this.UserInfo.Users;
	
}
```

Această funcție se conectează la funcția `Users` din server și preia datele tuturor utilizatorilor existenți precum este prezentat în imaginea de mai jos.

```
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
```

Informațiile utilizatorilor sunt formatate în format `json` în funcția `snprintf` în variabila de tip char `TempBuffer`.După care se pune o virgulă între datele utilizatorilor și se concatenează în variabila de tip char `MyBuffer` folosind funcția `strcat`.La final se trimite variabila `MyBuffer` în aplicația web și codul 200. Acestea sunt afișate pe ecran într-un tabel.


```
                <!--If the "ViewProducts" button was pressed this div will be displayed on screen-->
                <div class="ui basic segment" v-if="Menu.ViewProducts">
                    <div id="ShowSubmenuProducts">
                        <div class="ui top attached tabular menu">
                            <a class="item active" v-on:click="ShowSubmenuProducts('ViewProductsMenu')" id="ViewProducts">
                                View Products
                            </a>
                            <a class="item" v-on:click="ShowSubmenuProducts('DisableProductsMenu')" id="DisableProducts">
                                Disabled products
                            </a>

                        </div>
                        <div class="ui bottom attached segment" v-if="ProductsSubmenu.DisableProductsMenu">
                            <div class="row">
                                <table class="ui unstackable table">
                                    <thead>
                                        <tr>
                                            <th>ID</th>
                                            <th>Name</th>
                                            <th>Quantity</th>
                                            <th>Position</th>
                                            <th>Price</th>
                                            <th>isActive</th>
                                            <th>Edit product</th>
                                            <th>Enable product</th>
                                        </tr>
                                    </thead>
                                    <tbody v-for="product in products">                                        
                                        <tr v-if="!product.isActive">
                                            <td>{{product.ID}}</td>
                                            <td>{{product.Name}}</td>
                                            <td>{{product.Quantity}}</td>
                                            <td>{{product.Position}}</td>
                                            <td>{{product.Price}}</td>
                                            <td>{{product.isActive}}</td>
                                            <td style="text-align:center"><i class="edit outline icon disabled"></i></td>
                                            <td style="text-align:center"><i class="ban icon" v-on:click="EnableProduct(product)"
                                                 style="color:red;"></i></td>
                                        </tr>
                                    </tbody>
                                </table>
                            </div>
                        </div>

```


Apăsând iconița de edit se va deschide modal-ul de mai jos în care se pot edita informațiile utilizatorului.

 ![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/EditUsersInfoMenu.png)

Prin apăsarea butonului aceste informații ajung în funcția `EditUser` de unde sunt trimise pe server unde sunt prelucrate.Dacă prelucrarea s-a putut face atunci serverul va returna codul 200, în caz contrar va returna un cod de eroare.

```
async EditUser() {
            var toEditFirstName = document.getElementById("FirstName-modal").value;
            var toEditLastName = document.getElementById("LastName-modal").value;
            var toEditPassword = document.getElementById("Password-modal").value;
            var toEditCredit = document.getElementById("Credit-modal").value;
            var toEditAdmin = document.getElementById("CheckIfAdmin-modal");
            if (toEditAdmin.checked == true) {
                toEditAdmin = 1;
            } else {
                toEditAdmin = 0;
            }
            var toEditTagNumber = document.getElementById("TagNumber-modal").value;
            var CNP = this.TempUserEdit.CNP;
            if (toEditFirstName != "" && toEditLastName != "" && toEditCredit != "" &&  toEditTagNumber != "") {
                var editUrl = encodeURI("/EditUsers" + "?" + "CNP=" + CNP + "&" + "toEditFirstName=" 
                + toEditFirstName + "&" + "toEditLastName=" + toEditLastName + "&" + "toEditPassword=" 
                + toEditPassword + "&" + "toEditCredit=" + toEditCredit + "&" + "toEditAdmin=" + toEditAdmin + "&" 
                + "toEditTagNumber=" + toEditTagNumber);
                var editResult = await fetch(editUrl);
                console.log(editResult.status);
                if (editResult.status == 200) {
                    this.ShowModal("User's info has been updated!");
                }
                else {
                    this.ShowModal("Couldn't update user's info!");
                }
            } else {
                this.ShowModal("Fill all fields before submitting the form!");
            }
            this.GetUsers();
            $('.modal')
                .modal('hide')
                ;
            
        }
```

Mai jos este inserată o captură de ecran cu un exemplu de date ce pot fi inserate în câmpurile meniului de adăugarea a utilizatorilor.

![App Screenshot](https://raw.githubusercontent.com/Cozinia/VendingMachine/main/src/Poze%20proiect/AddNewUserMenu.png)
 
Adăugarea utilizatorilor se face trimițând datele inserate în formularul de mai jos pe server unde se execută funcția `AddUser` prezentată mai sus.

### Modul de funcționare al vendomatului

Modul de funcționare de bază:
Clientul introduce monede în vendomat, selectează produsul dorit, iar vendomatul returnează produsul.
Cum funcționează procesul de selecție:
* Butonul a fost apăsat
* Se verifică stocul produsului
* Se verifică dacă creditul inserat de utilizator este suficient pentru produsul selectat
* În cazul în care toate condițiile au fost îndeplinite vendomatul returnează produsul clientului
Vendomatul suportă 3 tipuri de monede(5, 10 și 50 de bani), 4 tipuri de produse din care utilizatorul poate alege și două moduri: standard și company.
În modul standard clientul inserează monede în vendomat, vede creditul introdus pe ecran, selectează produsul dorit , iar vendomatul îl returnează.
Pentru a vedea aplicația web din modul company este necesară conectarea la aceeași rețea ca cea a vendomatului.
În modul company utilizatorul scanează cardul RFID  pentru a se autentifica, selectează produsul dorit , iar vendomatul îl returnează.În acest mod de funcționare utilizatorul are un cont la care se poate loga și unde poate vedea tranzacțiile făcute și creditul rămas.
Modul de funcționare a sistemului de logare cu cardul RFID:
* Utilizatorul scanează cardul
* Un mesaj este afișat pe ecran
* După 3 secunde pe ecran este afișat creditul
* Începe procesul de selecție prezentat mai sus
* După terminarea procesului de selecție clientul apasă buton de logout 

Modul company vine și cu un cont de administrator unde acesta poate vedea, edita sau adăuga utilizatori și produse și nu în ultimul rând, poate testa funcționalitatea motoarelor.
	



