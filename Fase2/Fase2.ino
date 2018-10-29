#include <EEPROM.h>
#include "Arduino.h"
#include "Tacho.h"
//#include "LowPow.h"
#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(11,10); 

const int buttonStartPin = A1;
const int ledPin = A2;
int minutos = 36;
int minutosPrimeraLectura = 4;

int buttonStartState = 0;      
String FirstMedicion("");
String ResMedicion("");
Tacho r2d2=Tacho("983887060");
// estado II : 0
// estado O : 1
// estado 1 : 1

/* typeOfMessage could be ...
0: regular
1: alert
2: firstTime
*/
void mandar_SMS0(String mensaje){//mensaje a jeffrey
  Serial.println("Enviando SMS...");
  GSMSerial.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
  
  //GSMSerial.println("AT+CMGS=\"+51992547553\"\r");
  GSMSerial.println("AT+CMGS=\"+51944242562\"\r");
  delay(1000);
  GSMSerial.println(mensaje);
  delay(100);
  GSMSerial.println((char)26);//Comando de finalizacion ^Z
  delay(100);
  GSMSerial.println();
  delay(5000); // Esperamos un tiempo para que envíe el SMS
  Serial.println("SMS enviado");
}

void mandar_SMS2(String mensaje){ //mensaje a cesar
  Serial.println("Enviando SMS2...");
  GSMSerial.print("AT+CMGF=1\r");
  delay(1000); GSMSerial.println("AT+CMGS=\"+51940433430\"\r"); 
  delay(1000); GSMSerial.println(mensaje);
  delay(100); GSMSerial.println((char)26);
  delay(100); GSMSerial.println();
  delay(5000); 
  Serial.println("SMS2 enviado");
}
void mandar_SMS1(String mensaje){ //mensaje Stoyan
  Serial.println("Enviando SMS1...");
  GSMSerial.print("AT+CMGF=1\r");
  //984503610  Pocha
  delay(1000); GSMSerial.println("AT+CMGS=\"+51999729166\"\r"); 
  delay(1000); GSMSerial.println(mensaje);
  delay(100); GSMSerial.println((char)26);
  delay(100); GSMSerial.println();
  delay(5000); 
  Serial.println("SMS1 enviado");
}

void mandar_SMSPrueba(String mensaje){ //mensaje Paola
  Serial.println("Enviando SMSPrueba...");
  GSMSerial.print("AT+CMGF=1\r");
  //delay(1000); GSMSerial.println("AT+CMGS=\"+51943415889\"\r"); 
  delay(1000); GSMSerial.println("AT+CMGS=\"+51984503610\"\r");
  delay(1000); GSMSerial.println(mensaje);
  delay(100); GSMSerial.println((char)26);
  delay(100); GSMSerial.println();
  delay(5000); 
  Serial.println("SMSPrueba enviado");
}

void blinkLedPin(int times, int duration){
  for(int i = 0;i < times; i++){
     digitalWrite(ledPin, HIGH);
     delay(duration); 
     digitalWrite(ledPin, LOW);
     delay(duration); 
     
  }
}

void ledFade(boolean isInicializated){
  if(!isInicializated){
    blinkLedPin(3, 500);
  }
  else
  {
    digitalWrite(ledPin,HIGH);
  }
  return;
}

void llamada(){
  delay(5000);
  GSMSerial.print("ATD+51943415889;\r");
   // GSMSerial.print("ATD+51992547553;\r");
  Serial.println("llamando");
  delay(20000);
  Serial.println("finalizar llamada");
  GSMSerial.print("ATH\r");
}



void setup()
{   
 Serial.begin(9600); 
 GSMSerial.begin(9600);
 delay(5000);
 pinMode(buttonStartPin,INPUT);     
 pinMode(ledPin, OUTPUT);
 Serial.println("Start r2d2 program");
 delay(5000);
}

int cont1 = minutosPrimeraLectura;
int cont2 = minutos;
void loop(){  
    Serial.println(" >> enter r2d2code"); 
    //delay(3000);      
    buttonStartState = digitalRead(buttonStartPin); 
    Serial.print("buttonStartState: ");
    Serial.println(buttonStartState);
    if (buttonStartState == LOW )
    {
       
       Serial.println("Contador");
       Serial.println(cont1);
       Serial.println(FirstMedicion);
       blinkLedPin(cont1,1000);
       //for(int i = 0;i < 5;i++)      
       if (cont1 < minutosPrimeraLectura)
        {
        delay(60000);// 1 min
        cont1++;}
        else{
       FirstMedicion = r2d2.medir(1);
       Serial.println("FisrtMedicion: ");
        mandar_SMS0(FirstMedicion);
        mandar_SMS1(FirstMedicion);
        mandar_SMS2(FirstMedicion);
        blinkLedPin(4, 200);
          mandar_SMSPrueba(FirstMedicion);
        cont1 = 0;
        }
    }
    else
    { //funcionamiento normal
   
         Serial.println("Contador");
       Serial.println(cont2);
      blinkLedPin(cont2,1000);

         if (cont2 < minutos)
        {
        delay(60000);// 1 min
        cont2++;}
        else{
         ResMedicion = r2d2.medir(0);
         Serial.println("ResMedicion: ");
        Serial.println(ResMedicion);
          mandar_SMS0(ResMedicion);
          mandar_SMS1(ResMedicion);
          mandar_SMS2(ResMedicion);
          //llamada();
          blinkLedPin(4,200);
          mandar_SMSPrueba(ResMedicion);
          cont2 = 0;
        }        
     } 
 } 
