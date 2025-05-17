#include <Arduino.h>
#include <ESP8266WiFi.h>
//*Add Different Libraries for Initialization
#include "temp_sensor.h"
#include "tds_sensor.h"

//*for TDS Sensor
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define TDS_PIN A1
#define SCOUNT 30

//*for TEMP Sensor **likely combine with PH Sensor
#define TEMP_PIN 2

//*for Water Level Sensor
#define WTRLEVEL_PIN 3

void setup() {
  Serial.begin(9600);

  //*Initiate WIFI
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin("SHAW-E291", "kangaroos");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //*Initialize Sensors
  init_tempSensor();
  init_tdsSensor(TDS_PIN, VREF, SCOUNT);
  //Water Level Pin
  pinMode(WTRLEVEL_PIN,INPUT);
}

void loop() {

  //Read Sensors

  //Temperature Sensor
  float tempC = readTempC();
  //TDS Sensor
  setTemp_tdsSensor(tempC);
  update_tdsSensor();
  float tdsSensorReading = getValue_tdsSensor();
  //Water Level Sensor
  int wtrLevelReached = digitalRead(WTRLEVEL_PIN);

  
  //*test code 
  Serial.print("Temp Value: ");
  Serial.print(tempC, 0);
  Serial.println(" C");

  Serial.print("TDS Value: ");
  Serial.print(tdsSensorReading, 0);
  Serial.println(" ppm");

  Serial.print("Water level?");
  if(wtrLevelReached)
    Serial.print("At Water Level");
  else
    Serial.print("Not at Water Level");

  delay(2000);


}
