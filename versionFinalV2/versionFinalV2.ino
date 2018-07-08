#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "Arduino.h"
#include "Tacho.h"
#include <SoftwareSerial.h>
SoftwareSerial GSMSerial(11,10); 

const int buttonStartPin = A1;
const int ledPin = A2;

int buttonStartState = 0;      
String FirstMedicion("");
boolean flagfirstMedicion = false;
boolean flagSmsSend = false;
boolean flagTakeAction = false;

Tacho r2d2=Tacho("953857013");
int indexEE;
int eepromValue = 0;
long int iniTiempoInicio =0;
long int endTiempoInicio =0;
int potValue;

/* typeOfMessage could be ...
0: regular
1: alert
2: firstTime
*/
void mandar_SMS0(String mensaje){//mensaje a jeffrey
  Serial.println("Enviando SMS...");
  GSMSerial.print("AT+CMGF=1\r"); //Comando AT para mandar un SMS
  delay(1000);
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
  delay(1000); GSMSerial.println("AT+CMGS=\"+51943588606\"\r"); 
  delay(1000); GSMSerial.println(mensaje);
  delay(100); GSMSerial.println((char)26);
  delay(100); GSMSerial.println();
  delay(5000); 
  Serial.println("SMS2 enviado");
}
void mandar_SMS1(String mensaje){ //mensaje Pocha
  Serial.println("Enviando SMS1...");
  GSMSerial.print("AT+CMGF=1\r");
  delay(1000); GSMSerial.println("AT+CMGS=\"+51984503610\"\r"); 
  delay(1000); GSMSerial.println(mensaje);
  delay(100); GSMSerial.println((char)26);
  delay(100); GSMSerial.println();
  delay(5000); 
  Serial.println("SMS1 enviado");
}

void mandar_SMSPrueba(String mensaje){ //mensaje Maria
  Serial.println("Enviando SMSPrueba...");
  GSMSerial.print("AT+CMGF=1\r");
  delay(1000); GSMSerial.println("AT+CMGS=\"+51943415889\"\r"); 
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

void loop(){  
  indexEE = 0;      //Lee los valores de la EEPROM 
  indexEE += EEPROM_readAnything(indexEE, eepromValue);  
  indexEE += EEPROM_readAnything(indexEE, flagfirstMedicion); 
  Serial.println("flagfirstMedicion: ");
  Serial.println(flagfirstMedicion); 
  potValue = 4;
  Serial.print("potEstados: ");
  Serial.println(potValue);  
  if (eepromValue < potValue)
    {
      indexEE = 0; 
      eepromValue++;
      Serial.print("valor a escribir en EE ");
      Serial.println(eepromValue);
      blinkLedPin(eepromValue,500);
      indexEE +=EEPROM_writeAnything(indexEE, eepromValue);  //Guarda los valores en la EEPROM
      // solo para dejarlo seteado antes de poner en uso
     //flagfirstMedicion=0;
     //eepromValue= 15;
      indexEE +=EEPROM_writeAnything(indexEE, flagfirstMedicion);
      while(true)
      {
        digitalWrite(ledPin,HIGH);
        buttonStartState = digitalRead(buttonStartPin);
        Serial.println(buttonStartState);
     /* iniTiempoInicio = millis();  
      while (buttonStartState == HIGH)
      {
        buttonStartState = digitalRead(buttonStartPin);   
      }
      endTiempoInicio = millis();*/
      
      if (buttonStartState == HIGH)
      {
         FirstMedicion = r2d2.medir(1);
         mandar_SMS0(FirstMedicion);
         //mandar_SMS1(FirstMedicion);
         blinkLedPin(4, 200);
         //mandar_SMS2(FirstMedicion);
         mandar_SMSPrueba(FirstMedicion);
         Serial.println("FisrtMedicion: again ");//debugging
         Serial.println(FirstMedicion); //debugging
         flagfirstMedicion = true;
         flagTakeAction = true;
      }
      
      }
    }
  else
    {
      Serial.println(" >> enter r2d2code"); 
      delay(3000);      
      while(!flagTakeAction)
      {
        ledFade(flagfirstMedicion);
        buttonStartState = digitalRead(buttonStartPin); 
        Serial.print("buttonStartState: ");
        Serial.println(buttonStartState);
        if (buttonStartState == HIGH )
        {
          if( flagfirstMedicion == false)
            {//buttonStartState is pressed (primera vez)
             FirstMedicion = r2d2.medir(1);
             mandar_SMS0(FirstMedicion);
             //mandar_SMS1(FirstMedicion);
             blinkLedPin(4, 200);
            mandar_SMSPrueba(FirstMedicion);
             //mandar_SMS2(FirstMedicion);
             Serial.println("FisrtMedicion: ");//debugging
             Serial.println(FirstMedicion); //debugging
             flagfirstMedicion = true;
             flagTakeAction = true;
             }
        }
        else
        { //funcionamiento normal
        if(flagfirstMedicion){
           if(flagSmsSend == false)
            {
             String resMedicion = r2d2.medir(0);
             mandar_SMS0(resMedicion);
             //mandar_SMS1(resMedicion);
             //mandar_SMS2(resMedicion);
             //llamada();
             Serial.println("resMedicion:  ");//debugging
             Serial.println(resMedicion);//debugging
             flagSmsSend = true;
             blinkLedPin(4,200);
             mandar_SMSPrueba(resMedicion);
             flagTakeAction = true;
            }  
         } 
        }
      
      eepromValue=0;
      indexEE = 0;
      Serial.println("Reiniciar a 0");
      indexEE += EEPROM_writeAnything(indexEE, eepromValue);      
      indexEE += EEPROM_writeAnything(indexEE, flagfirstMedicion);
      while(true)
        {
          digitalWrite(ledPin,HIGH);
          buttonStartState = digitalRead(buttonStartPin);
          Serial.println(buttonStartState);
          /*iniTiempoInicio = millis();  
          while (buttonStartState == HIGH)
            {
              buttonStartState = digitalRead(buttonStartPin);   
            }
          endTiempoInicio = millis();*/
          
          if (buttonStartState == HIGH)
            {
               FirstMedicion = r2d2.medir(1);
               mandar_SMS0(FirstMedicion);
               //mandar_SMS1(FirstMedicion);
               blinkLedPin(4, 200);
              mandar_SMSPrueba(FirstMedicion);
               //mandar_SMS2(FirstMedicion);
               Serial.println("FisrtMedicion: again ");//debugging
               Serial.println(FirstMedicion); //debugging
               flagfirstMedicion = true;
               flagTakeAction = true;
            }
        }//end while
      } //end while
    } // end else    
}//end loop
