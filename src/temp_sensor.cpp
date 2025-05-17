#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temp_sensor.h"

#define TEMP_PIN 2  // Adjust this to your actual pin

OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

void init_tempSensor() {
  sensors.begin();
}

float readTempC() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return temp;
}