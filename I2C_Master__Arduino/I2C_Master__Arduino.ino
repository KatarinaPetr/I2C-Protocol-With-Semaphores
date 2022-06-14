// I2C library.
#include <Wire.h>

// pin for each color
#define RED 8
#define YELLOW 7
#define GREEN 6

#define SLAVE_ADDR 9

bool start = false;
String input = ""; // type "start" to see what happens...

byte x; //1 for red, 2 for yellow, 3 for green, 4 for yellow and red

void setup() {
	pinMode(RED,OUTPUT);
  digitalWrite(RED,LOW);
  pinMode(YELLOW,OUTPUT); 
  digitalWrite(YELLOW,LOW);
  pinMode(GREEN,OUTPUT); 
  digitalWrite(GREEN,LOW);

  Serial.begin(9600);
	Wire.begin();
}

void loop() {	
  x=1;
  onStart();
  if (start){
  	// RED 
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(RED,HIGH);
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(x); // passing value to slave
    Wire.endTransmission();
    receved(); // see what we got from slave for print
    x=2;
    delay(9000);
    // YELLOW
    digitalWrite(YELLOW,HIGH);
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(x); // passing value to slave 
    Wire.endTransmission();
    receved();
    x=3;
    delay(3000);
    digitalWrite(RED,LOW);
    digitalWrite(YELLOW,LOW);
    
    //GREEN
    digitalWrite(GREEN,HIGH);
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(x); // passing value to slave
    Wire.endTransmission();
    receved();
    x=4;
    delay(9000);
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(x); // passing value to slave (to know when to turn YELLOW on)
    Wire.endTransmission();
    receved();
    delay(3000);
    digitalWrite(GREEN,LOW);
    x=2;
    
    //YELLOW
    digitalWrite(YELLOW,HIGH);
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(x); // passing value to slave
    Wire.endTransmission();
    receved();
    delay(1500);
    digitalWrite(YELLOW,LOW);
    }
    // if not started
    else { 
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED,LOW);
    }
}

void onStart(){
  if(Serial.available()>0){
    input=Serial.readStringUntil('\n');
    if (input.equals("start")){
      start = true;
      Serial.println("TRANSFER HAS STARTED!");
    }else{
      start = false;
    }
  }else{
    input="";
  }
}

void receved(){
  Wire.requestFrom(SLAVE_ADDR,1);
  if(Wire.available()){ // Slave send something
    int number = Wire.read();
    //1 for green, 2 for yellow, 3 for red
    if (number == 1) {
      Serial.println("SEMAPHORE 2 -> ALLOWED PASSAGE!");
    }else if (number == 2) {
      Serial.println("SEMAPHORE 2 -> FORBIDDEN PASSAGE!");
    }else if (number == 3) {
      Serial.println("SEMAPHORE 2 -> FORBIDDEN PASSAGE!");
    }else if (number == 4 ) {
      Serial.println("SEMAPHORE 2 -> FORBIDDEN PASSAGE!");
    }else {
      Serial.println("SEMAPHORE 2 -> DEAD!");
    }
  }
}
