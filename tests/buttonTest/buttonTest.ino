const int buttonPin = A3;
const int ledPin =  A2; 

int buttonState = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT);     
}

void loop(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("YES");    
    digitalWrite(ledPin, HIGH);  
  } 
  else {
        Serial.println("NO");
    digitalWrite(ledPin, LOW); 
  }
}
