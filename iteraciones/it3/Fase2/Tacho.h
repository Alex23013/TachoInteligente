#include "US.h"
class Tacho{
  Ultrasonic ultrasonic[3];
  int valores[3];
  String numSen;
public:
  Tacho(String numSen)
  {
    this->numSen = numSen;
    ultrasonic[0]=Ultrasonic(3,2);
    ultrasonic[1]=Ultrasonic(5,4);
    ultrasonic[2]=Ultrasonic(7,6);
  }
  String medir(int typeOfMessage) 
  {
    String resMedicion("{ \"numSen\": \"");
    resMedicion.concat(numSen);
    resMedicion.concat("\", ");
    int contador=0;
    Serial.println();
    if(typeOfMessage == 2)
    {
    resMedicion.concat("\"sensor1\": 0, \"sensor2\": 0, \"sensor3\": 0, ");
    }
    else{
     for(int i=0;i<3;i++){
        valores[i]=0;
        int ival=0;
        int ifal=0;
        while(ival<3 && ifal<7){
          int aux=ultrasonic[i].Promedio();
          if(aux){
            valores[i]+=aux;
            ival++;
          }else{
            ifal++;
          }
        }
        if(ifal>=7){
          valores[i]=0;
        }else{
          valores[i]/=3;
        }
        
        Serial.print(" - "); //only for debugging
        Serial.print(valores[i]); //only for debugging
      
        String val(valores[i]);
        resMedicion.concat("\"sensor");
        resMedicion.concat(i+1);
        resMedicion.concat("\": ");
        resMedicion.concat(val);
        if(i<3)resMedicion.concat(" ,");
        if(i==3)resMedicion.concat(" ");
      }
      
      Serial.println();
    }
    
    resMedicion.concat("\"button\": ");
    resMedicion.concat(typeOfMessage);
    resMedicion.concat(" }");
    return resMedicion;///
  }
};

