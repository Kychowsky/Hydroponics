#include <Arduino.h>
//*Add Different Libraries for Initialization
#include "temp_sensor.h"
#include "tds_sensor.h"

//*for TDS Sensor
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define TDS_PIN A1
#define SCOUNT 30

void setup() {
  //*Begin Serial Communication at 9600 BAUD 
  Serial.begin(9600);
  init_tempSensor();
  init_tdsSensor(TDS_PIN, VREF, SCOUNT);
}

void loop() {

  //*Read Sensors
  float tempC = readTempC();
  setTemp_tdsSensor(tempC);
  float tdsSensorReading = getValue_tdsSensor();
  
  //*test code 
  Serial.print("Temp Value: ");
  Serial.print(tempC, 0);
  Serial.println(" C");

  Serial.print("TDS Value: ");
  Serial.print(tdsSensorReading, 0);
  Serial.println(" ppm");
  delay(2000);


}
