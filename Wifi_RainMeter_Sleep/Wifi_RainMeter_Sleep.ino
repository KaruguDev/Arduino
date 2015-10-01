
#include <Sleep_n0m1.h>
#include <SoftwareSerial.h>
#include "WiFly.h"
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include <RTCTimer.h>

#define SSID      "Belkin"
#define KEY       "upande-gani$"
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK
#define WIFI_SHIELD_PIN 3
#define RAIN_PIN 10
#define RAIN_INT 2

float RainAccum=0.0;
int tips = 0;
float rain_factor = 0.2794;

RTCTimer timer;

Sleep sleep;
unsigned long sleepTime; //how long you want the arduino to sleep


 
SoftwareSerial uart(2, 3); // create a serial connection to the WiFi shield TX and RX pins.
WiFly wifly(&uart); // create a WiFly library object using the serial connection to the WiFi shield we created above.

void setup(){

  Serial.begin(9600); // start the Arduino serial monitor window connection
  Wire.begin();
  rtc.begin();
  
  sleepTime = 50000;
  uart.begin(9600); // start the serial connection to the shield

   // Do the work every number of seconds
  timer.every(60 , takedata);

  // Instruct the RTCTimer how to get the current timestamp
  timer.setNowCallback(getNow);
 
  //Switch on the second Grove column
  pinMode(RAIN_PIN, INPUT_PULLUP);
  attachInterrupt(RAIN_INT, tipInterrupt, CHANGE); //D10 Yellow Grove pin 1 

  //establishing WIFI connection
  pinMode(WIFI_SHIELD_PIN, OUTPUT);
  while (wifly.join(SSID, KEY, AUTH)== true) {
      Serial.println("Failed to join " SSID);
      wifly.reset();
      delay(100);
    }
    Serial.println("Connection successful");
    wifly.save();    // save configuration, 
  }
    
void takedata(uint32_t ts)
{
  RainAccum = tips * rain_factor;
  Serial.print("There has been ");
  Serial.print(RainAccum);
  Serial.println(" mm of rain");
      
  
  tips = 0;
  RainAccum = 0.00;
  
}

void tipInterrupt()
{
  tips ++; 
}

uint32_t getNow()
{
  return rtc.now().getEpoch();
}

void loop(){

  while (!wifly.available()) {
      Serial.println("Beginning");
      digitalWrite(WIFI_SHIELD_PIN, HIGH);//switches on the LED
      Serial.println("I am WIFLY and I am active");
      timer.update();
      delay(500); //delay to allow serial to fully print before sleep
      Serial.println("going to sleep");
      delay(100);
      digitalWrite(WIFI_SHIELD_PIN, LOW);//switches off the LED
      sleep.pwrDownMode(); //set sleep mode
      sleep.sleepDelay(sleepTime); //sleep for: sleepTime
    }
  Serial.println("Something wrong with the connection");


}

