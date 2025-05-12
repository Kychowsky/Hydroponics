#include <Arduino.h>
//*Add Different Libraries for Initialization
#include "temp_sensor.h"
#include "tds_sensor.h"

//*for TDS Sensor
#define VREF 5.0      // analog reference voltage(Volt) of the ADC

void setup() {
  //*Begin Serial Communication at 9600 BAUD 
  Serial.begin(9600);
  initTempSensor();
}

void loop() {

  //*Read Temp Code
  float tempC = readTempC();



}
