#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temp_sensor.h"

#define ONE_WIRE_BUS 2  // Adjust this to your actual pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void initTempSensor() {
  sensors.begin();
}

float readTempC() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return temp;
}