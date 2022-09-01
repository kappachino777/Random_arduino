#include<LiquidCrystal.h>
const int ledPin1 = 13;
const int ledPin2 = 10;
const int ledPin3 = 9;
const int sensorPin1 = 0;
const int sensorPin2 = 1;
const int sensorPin3 = 6;
const int startPin = 8;
const int TIMEOUT = 1000000; // milliseconds
float timeStart;
float timeFinish;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Setup runs once, at start
// Input and Output pins are set
void setup(){
   lcd.begin(16,2);
   pinMode(sensorPin1, INPUT);
   pinMode(sensorPin2, INPUT);
   pinMode(sensorPin3, INPUT);
   pinMode(startPin, INPUT);
   pinMode(ledPin1, OUTPUT);
   pinMode(ledPin2, OUTPUT);
   pinMode(ledPin3, OUTPUT);
}

// Called repeatedly
void loop() {
   // Get the Sensor status
   int status1 = digitalRead(sensorPin1);
   int status2 = digitalRead(sensorPin2);
   int status3 = digitalRead(sensorPin3);
   int start = digitalRead(startPin);
   if (start == HIGH){
    timeStart = millis();
   }
   else if (status1 == HIGH || status2 == HIGH || status3 == HIGH) {
        // A sensor was tripped, show the results until timeout
        digitalWrite(ledPin1, status1);
        digitalWrite(ledPin2, status2);
        digitalWrite(ledPin3, status3);
        timeFinish = (millis()-timeStart)/1000;
        if(status1==HIGH){
          lcd.setCursor(0,0);
          lcd.print("player 1 win");
          lcd.setCursor(0,1);
          lcd.print(timeFinish);
          lcd.setCursor(13,1);
          lcd.print("Sec");
        }
        else if(status2==HIGH){
          lcd.setCursor(0,0);
          lcd.print("player 2 win");
          lcd.setCursor(0,1);
          lcd.print(timeFinish);
          lcd.setCursor(13,1);
          lcd.print("Sec");
        }
        else if(status3==HIGH){
          lcd.setCursor(0,0);
          lcd.print("player 3 win");
          lcd.setCursor(0,1);
          lcd.print(timeFinish);
          lcd.setCursor(13,1);
          lcd.print("Sec");
        }
        else{
          lcd.setCursor(0,0);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("                ");
        }
        timeStart=0;
        delay(TIMEOUT); // Wait for timeout
     }
   
} 
