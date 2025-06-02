#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <OneWire.h>
#include <ADS1X15.h>
#include <Wire.h>
#include <DallasTemperature.h>

// --- Pin and Channel Configuration ---
#define TEMP_PIN 12            // GPIO12 (D6)
#define TDS_ADS_CHANNEL 0      // ADS1115 A0
#define PH_ADS_CHANNEL 1       // ADS1115 A1
#define SAMPLE_COUNT 40

// --- ADC Gain and Calibration Constants ---
const float ADC_GAIN = 6.144;         // for 5V reference on ADS1115
const int ADC_RESOLUTION = 32767;     // 16-bit (signed) ADC
const float gainMultiplier = ADC_GAIN / ADC_RESOLUTION;

// --- pH Calibration (based on measured voltages for known pH) ---
const float m = -5.96;
const float C = 22.764;

// --- External Sensor Instances ---
extern OneWire oneWire;
extern DallasTemperature sensors;
extern ADS1115 ads;

// --- Public Function Declarations ---
void init_sensors();

int readAdsPH();
int readAdsTDS();

float readTempC();
float readPH();
float readTDS();

int getAveragedADC(int (*readFunc)());
int getMedianNum(int bArray[], int iFilterLen);

#endif // SENSORS_H