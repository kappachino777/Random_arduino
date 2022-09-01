#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);
int val;
int tempPin = 1; // pin data temperatur
float mv;
float cel;
int encoder_pin = 2; // pin encoder
unsigned int rpm; // rpm reading
volatile byte pulses; // number of pulses
unsigned long timepid; // sampling time PID
unsigned int pulsesperturn = 20; // banyaknya pulse per putaran
const int IN1 = 7; // untuk input driver
const int IN2 = 6; // untuk input driver
const int ENA = 9; // untuk input PWM driver
int setPointSpeed;
int elapsedTime;
float error;
float cumError;
float lastError;
float rateError;
float PIcon;
float kp;
float ki;
float kd;
float mappedRPM; 
String condition;
void counter()
{
  //Update count
  pulses++;
}

void setup() {
  Serial.begin(9600); 
  lcd.init();
  lcd.backlight();
  //———–code for RPM—————
  pinMode(encoder_pin, INPUT); // koneksi pin encoder
  //Triggers on FALLING (change from HIGH to LOW)
  attachInterrupt(0, counter, FALLING);
  // Initialize
  val = analogRead(tempPin);
  mv = ( val/1023.0)*5000;
  cel = mv/10;
  pulses = 0;
  rpm = 0;
  timepid = 0;
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  // nilai koefisien PID
  kp=1;
  ki=0.0001;
  kd=0.0001;
  lastError=0;
}
void loop()
{
if (millis() - timepid >= 100){
  //ts PID per 0.1 detik atau 100ms
  cel=0;
  val = analogRead(tempPin);
  mv = (val/1024.0)*5000;
  cel = mv/10;
  //penentuan setpoint berdasarkan suhu ruangan
  //setpoint berupa output PWM
  if (cel<25){
    setPointSpeed=127;
    condition ="cold";
  }
  if (25<=cel<=33){
    setPointSpeed=191;
    condition ="med";
  }
  if (cel>33){
    setPointSpeed=255;
    condition ="hot";
  }
  detachInterrupt(0); //interupt dihentikan ketika kalkulasi
  rpm = (60*1000/pulsesperturn)/(millis()-timepid)*pulses;
  pulses = 0;
  attachInterrupt(0, counter, FALLING);
  //dari pengujian RPM maksimal dari motor adalah 400
  //sehingga nilai RPM di mapping dari 0-1000 ke 0-255
  //untuk input ke PWM driver
  mappedRPM=map(rpm,0,1000,0,255);
  //penghitungan PID
  elapsedTime=millis() - timepid;
  if (condition=="cold"){
    error=setPointSpeed-mappedRPM;
    cumError += error*elapsedTime;
    rateError = (error - lastError)/elapsedTime;
    PIcon=kp*error+ki*cumError+kd*rateError;
    lastError=error;
}
  else if (condition=="med"){
    error=setPointSpeed-mappedRPM;
    cumError += error*elapsedTime;
    rateError = (error - lastError)/elapsedTime;
    PIcon=kp*error+ki*cumError+kd*rateError;
    lastError=error;
}
  else if (condition=="hot"){
    error=setPointSpeed-mappedRPM;
    cumError += error*elapsedTime;
    rateError = (error - lastError)/elapsedTime;
    PIcon=kp*error+ki*cumError+kd*rateError;
    lastError=error;
}
  // untuk menghindari nilai negatif karena input PWM tidak bisa negatif
  if (PIcon<0){
    PIcon=0;
  }
  timepid = millis();
}
analogWrite(ENA,PIcon);//mengirimkan hasil PID ke driver
lcd.setCursor(0,0);
lcd.print("                ");
lcd.setCursor(0,0);
lcd.print("SP");
lcd.setCursor(3,0);
lcd.print(condition);
lcd.setCursor(7,0);
lcd.print("RPM");
if (rpm<1000){
  lcd.setCursor(12,0);
  lcd.print(rpm);
}
else if(rpm>=1000){
  lcd.setCursor(11,0);
  lcd.print(rpm);
}
lcd.setCursor(0,1);
lcd.print("temp=");
lcd.setCursor(6,1);
lcd.print(cel);
}
