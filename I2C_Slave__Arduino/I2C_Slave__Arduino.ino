
// I2C library.
#include <Wire.h>

// pin for each color
#define RED 8
#define YELLOW 7
#define GREEN 6

#define MASTER_ADDR 9

bool timeOut= false; // case yellow lights apart from red or not
int answer = 0; // num from master

void receive_event(){ // from I2C_Master
  while(Wire.available()>0){
    byte number = Wire.read(); 
    answer = (int)number;
    Wire.onRequest(request_event);
    
    /* if RED(1) -> GREEN
       if YELLOW(2) -> YELLOW
       if GREEN(3) -> RED
       if GREEN(4) -> RED & YELLOW */ 
   
    if (number == 1) { 
      Serial.println("SEMAPHORE 1 -> FORBIDDEN PASSAGE!");
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, LOW);
      digitalWrite(GREEN, HIGH);
      timeOut=false;
    }
    else if (number == 2) { 
      if (!timeOut){
        Serial.println("SEMAPHORE 1 -> FORBIDDEN PASSAGE!");
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, LOW);
        digitalWrite(YELLOW, HIGH);
      }else{
        Serial.println("SEMAPHORE 1 -> FORBIDDEN PASSAGE!");
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, HIGH);
        digitalWrite(YELLOW, HIGH);
      }
    }
    else if (number == 3) {
      Serial.println("SEMAPHORE 1 -> ALLOWED PASSAGE!");
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED,HIGH);
      timeOut=true;
    }
    else if (number == 4) { // when green is about to turn off on master!
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      digitalWrite(YELLOW, HIGH);
    } 
    else {
      Serial.println("SEMAPHORE 1 -> DEAD!");
      digitalWrite(RED,LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
    }
  }
}

void request_event(){ // to I2C_Master
  int response;
  response = answer;
  Wire.write(response);
  //Serial.println(response);
}

void setup() {
  // init
	pinMode(RED,OUTPUT); 
  digitalWrite(RED,LOW);
  pinMode(YELLOW,OUTPUT); 
  digitalWrite(YELLOW,LOW);
  pinMode(GREEN,OUTPUT); 
  digitalWrite(GREEN,LOW);

	Wire.begin(MASTER_ADDR);
  Serial.begin(9600);
 
  Wire.onReceive(receive_event);
}

void loop() {
	
}
