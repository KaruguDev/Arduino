#include <Wire.h>
#include <Sodaq_DS3231.h>
#include <RTCTimer.h>

float RainAccum=0.0;
int tips = 0;
float rain_factor = 0.2794;

RTCTimer timer;

void setup()
{
  //Setup the Serial Monitor
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();

   // Do the work every number of seconds
  timer.every(20 , takedata);

  // Instruct the RTCTimer how to get the current timestamp
  timer.setNowCallback(getNow);
  
  //Switch on the second Grove column
  pinMode(10, INPUT_PULLUP);
  attachInterrupt(2, tipInterrupt, CHANGE); //D10 Yellow Grove pin 1
}

void takedata(uint32_t ts)
{
  RainAccum = tips * 0.5 * rain_factor;
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

void loop()
{
  timer.update();
}
