#include <Arduino.h>
#include <OneWire.h>
#include <ADS1X15.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "ph-temp_sensor.h"


#define TEMP_PIN 12  // GPIO12 (D6)
//#define PH_PIN 3 


//using the ADS1115(16-bit resolution) which transmits the voltage via a reading
const float ADC_REF = 5.0;         // 5V reference  on ADS1115 and PH-Sensor
const int ADC_RESOLUTION = 32768;   // 16-bit ADC (0–32767)
//Calculated Slope/Intercept based on 7.8pH-2.51V and 3.5pH-3.4V
const float m = -5.96;
const float C = 22.764;

//Configure ONEWIRE
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

//Configure ADS1115
ADS1115 ads(0x48); //Default Address
void setup_ads(){
  Wire.begin(D2,D1); // For ESP8266: SDA = D2 (GPIO4), SCL = D1 (GPIO5) for I2C
    if (!ads.begin()) {
      Serial.println("ADS1115 not found!");//I need error output on webpage
    }
    else {
      Serial.println("ADS found!");
    }
}

void init_tempSensor() {
  sensors.begin();
}

float readTempC() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return temp;
}

float readPH(){
  int rawValue = analogRead(PH_PIN);                      // Read raw value
  float phVoltage = (rawValue * ADC_REF) / ADC_RESOLUTION;  // Convert to volts
  float phValue = phVoltage*m+C;
  return phValue;
}