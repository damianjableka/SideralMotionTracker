#include <Pcf.h>
#include <Wire.h>

#define IlePinow 4
#define KrokNaSek 4


// Initialize a PCF8574 at I2C-address 0x20, using GPIO5, GPIO4 and testWire for the I2C-bus
Pcf pcf = Pcf(0x20);

//odwrotnie bo prowadzi w druga strone //{D5,D7,D8,D6}
int piny[IlePinow]={0,1,2,3};       //piny uzyte do sterowania w odpowiendniej kolejnosci uzytej w sekwencji
int sekwencja[KrokNaSek][IlePinow]={    //sekwencja pinow   // do zmiany bo wystarczy aby to zmienialo te piny ktore trzeba zmienic a nie wszystkie bedzie for o polowe mniej operacji
  {1,0,0,1},
  {HIGH,LOW,HIGH,LOW},
  {LOW,HIGH,HIGH,LOW},
  {LOW,HIGH,LOW,HIGH}};



unsigned long czas;   //czas rozpoczecia dzialania funkcji loop w microsekundach odswierzany przy kazdej expozycji
unsigned long czekaj_do = 29681;
//unsigned long zegarek;
int Krok=0;
int k;
unsigned long CzasP; 
unsigned long poprawka=0; 
unsigned long CoIlePoprawka = 100;
int CoIleP=(int)CoIlePoprawka;
void setup() {
  
  
 Serial.begin(9600);
  pcf.writeData(255); // turns on all 8 pins
  delay(1000);
  pcf.clearData(); // turns off all 8 pins
  
 // Wire.pins(D2, D1);
// Wire.begin();
//Serial.begin(115200);
  //expander.begin(0x20);
  //Specsheets say PCF8574 is officially rated only for 100KHz I2C-bus
  //PCF8575 is rated for 400KHz

 
  for(k=0;k<IlePinow;k++){
     // pcf.writePin(piny[k], OUTPUT);
      pcf.writePin(piny[k], HIGH);
     
     
  }
delay(1000);
    pcf.writePin(0, LOW);
delay(1000);
     pcf.writePin(1, LOW);
    delay(1000);
     pcf.writePin(2, LOW);
    delay(1000);
     pcf.writePin(3, LOW);
    delay(1000);
     pcf.writePin(2, HIGH);
    delay(1000);
     pcf.writePin(1, HIGH);
    delay(1000);
     pcf.writePin(3, HIGH);
    delay(1000);
     pcf.writePin(0, HIGH);

czas=micros();
CzasP=micros();
}



void loop() {



  
  if((micros()-czas+poprawka)>czekaj_do)
        {


          for(k=0;k<IlePinow;k++){  
              pcf.writePin(piny[k], sekwencja[Krok%4][k]);
          }
          Serial.println(poprawka);
      Serial.println(micros()-czas);
      
      
      
      if((Krok%CoIleP==0)&&(Krok>0))
        {
         poprawka+=((micros()-CzasP)/CoIlePoprawka)-czekaj_do;
         Serial.println("POPRAWKA");
         Serial.println((micros()-CzasP)/CoIlePoprawka);
         CzasP=micros();
          Serial.println(poprawka);
          Serial.println("POPRAWKA");
        }               
            czas=micros();
          Krok++;  
        }

}
