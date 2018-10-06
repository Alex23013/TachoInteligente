#include "Arduino.h"
#include "Tacho.h"
#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(11,10); 

const int buttonStartPin = A1;
const int ledPin = A2;
int minutes = 36;
int minutesFirstLecture = 4;

int buttonStartState = 0;      
String firstMedicion("");
String resMedicion("");
Tacho r2d2=Tacho("983887060");

/* typeOfMessage could be ...
0: regular
1: alert
2: firstTime
*/
void mandar_SMS0(String mensaje){//mensaje a jeffrey
  Serial.println("Enviando SMS...");
  GSMSerial.print("AT+CMGF=1\r"); 
  delay(1000);
  
  GSMSerial.println("AT+CMGS=\"+51944242562\"\r");
  delay(1000);
  GSMSerial.println(mensaje);
  delay(100);
  GSMSerial.println((char)26);
  delay(100);
  GSMSerial.println();
  delay(5000); 
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

void blinkLedPin(int times, int duration){
  for(int i = 0;i < times; i++){
     digitalWrite(ledPin, HIGH);
     delay(duration); 
     digitalWrite(ledPin, LOW);
     delay(duration); 
     
  }
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

int cont1 = minutesFirstLecture;
int cont2 = minutes;
void loop(){  
    Serial.println(" >> enter r2d2code"); 
    buttonStartState = digitalRead(buttonStartPin); 
    Serial.print("buttonStartState: ");
    Serial.println(buttonStartState);
    if (buttonStartState == LOW )
    {
       
       Serial.println("Contador");
       Serial.println(cont1);
       Serial.println(firstMedicion);
       blinkLedPin(cont1,1000);    
       if (cont1 < minutesFirstLecture)
        {
        delay(60000);// 1 min
        cont1++;}
        else{
       firstMedicion = r2d2.medir(1);
       Serial.println("FisrtMedicion: ");
        mandar_SMS0(firstMedicion);
        mandar_SMS1(firstMedicion);
        mandar_SMS2(firstMedicion);
        blinkLedPin(4, 200);
        cont1 = 0;
        }
    }
    else
    { //funcionamiento normal
   
         Serial.println("Contador");
       Serial.println(cont2);
      blinkLedPin(cont2,1000);

         if (cont2 < minutes)
        {
        delay(60000);// 1 min
        cont2++;}
        else{
         resMedicion = r2d2.medir(0);
         Serial.println("resMedicion: ");
        Serial.println(resMedicion);
          mandar_SMS0(resMedicion);
          mandar_SMS1(resMedicion);
          mandar_SMS2(resMedicion);
          blinkLedPin(4,200);
          cont2 = 0;
        }        
     } 
 } 
