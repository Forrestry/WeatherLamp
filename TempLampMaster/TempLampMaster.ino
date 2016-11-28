// api address
//http://api.openweathermap.org/data/2.5/forecast/city?id=5525577&APPID=d138b452a0709c22d27f2636b1fea9aa&mode=xml
//load shortcuts for arduino ethernet shield
//need to parse xml page in c++
#include <SPI.h>
#include<Ethernet.h>


//Creating variables for pins to use
int highTemp = 3;
int medTemp = 5;
int lowTemp = 6;
int rain = 2;
int thunder = 9;
// Just creating integer variables to use later
int T = 50;
float hT = 0;
float mT = 0;
float lT = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(highTemp,OUTPUT);
  pinMode(medTemp,OUTPUT);
  pinMode(lowTemp,OUTPUT);
  pinMode(rain, OUTPUT);
  pinMode(thunder,OUTPUT);
}

void loop() {

  //goes after reading api data
  // if statements are put in to include all temperatures
  // For T> some temp, only high temp LED is lit
if (T > 90) {
  digitalWrite(highTemp, HIGH);
  digitalWrite(medTemp, LOW);
  digitalWrite(lowTemp, LOW);
}
// For the higher real range of T, high and medium temp LED's vary in brightness
if (T <= 90 && T >= 70) {
  //Brightness values for each temperature change linearly
  hT = ( (float)T - 70 ) / 20;
  mT = ( 90 - (float)T ) / 20;
  //These values are then applied through PWM
  analogWrite(highTemp, hT*255);
  analogWrite(medTemp, mT*255);
  digitalWrite(lowTemp, LOW);
}
//  for the median real number range of T, only medium LED is on
if (T < 70 && T > 60){
  digitalWrite(highTemp, LOW);
  digitalWrite(medTemp, HIGH);
  digitalWrite(lowTemp, LOW);
}
// For the lower real range of T, medium and low temp LED's vary in brightness
if (T <= 60 && T >= 40) {
  mT = ( (float)T - 40 ) / 20;
  lT = ( 60 - (float)T ) / 20;
  digitalWrite(highTemp, LOW);
  digitalWrite(medTemp, mT*255);
  digitalWrite(lowTemp, lT*255);
}
// For T< some temp, only low temp LED is on
if (T < 40) {
  digitalWrite(highTemp, LOW);
  digitalWrite(medTemp, LOW);
  digitalWrite(lowTemp, HIGH);
}
//rain goes here.
//for these I need to know terms api uses.
//Put thunder after temperature and rain. Will use for loop to get more realistic lightning effect
delay(1000);
}
