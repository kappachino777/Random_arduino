// Include the required Arduino libraries:
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CS_PIN 3

// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
const int ledPin1 = 13;
const int ledPin2 = 12;
const int ledPin3 = 11;
const int sensorPin1 = 4;
const int sensorPin2 = 5;
const int sensorPin3 = 6;
const int startPin = 2;
const int TIMEOUT = 1000000; // milliseconds
int trigger;
unsigned long timenow;
unsigned long finishtime;
unsigned long timestart;
void setup(){
  // Intialize the object:
  myDisplay.begin();
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(0);
  // Clear the display:
  myDisplay.displayClear();
   pinMode(sensorPin1, INPUT);
   pinMode(sensorPin2, INPUT);
   pinMode(sensorPin3, INPUT);
   pinMode(startPin, INPUT);
   pinMode(ledPin1, OUTPUT);
   pinMode(ledPin2, OUTPUT);
   pinMode(ledPin3, OUTPUT);
   attachInterrupt(digitalPinToInterrupt(startPin), start, FALLING);
}
void start(){
  timestart=millis();
  trigger = 1;
}
// Called repeatedly
void loop() {
   // Get the Sensor status
   int status1 = digitalRead(sensorPin1);
   int status2 = digitalRead(sensorPin2);
   int status3 = digitalRead(sensorPin3);
   if (trigger == 1){
    timenow=(millis()-timestart);
    myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print(timenow);
    if (status1 == HIGH || status2 == HIGH || status3 == HIGH) {
        // A sensor was tripped, show the results until timeout
        digitalWrite(ledPin1, status1);
        digitalWrite(ledPin2, status2);
        digitalWrite(ledPin3, status3);
        if(status1==HIGH){
          finishtime = millis()-timestart;
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print("P1 Win");
          delay(2000);
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print(finishtime);
          delay(TIMEOUT); // Wait for timeout
          trigger=0;
          myDisplay.displayClear();
        }
        else if(status2==HIGH){
          finishtime = (millis()-timestart);
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print("P2 Win");
          delay(2000);
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print(finishtime);
          delay(TIMEOUT); // Wait for timeout
          trigger=0;
          myDisplay.displayClear();
        }
        else if(status3==HIGH){
          finishtime = millis()-timestart;
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print("P3 Win");
          delay(2000);
          myDisplay.setTextAlignment(PA_CENTER);
          myDisplay.print(finishtime);
          delay(TIMEOUT); // Wait for timeout
          trigger=0;
          myDisplay.displayClear();
        }
     }
   }
} 
