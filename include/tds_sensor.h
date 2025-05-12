#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include <Arduino.h>

// Call in your sketch’s setup()
void init_tdsSensor(uint8_t analogPin, float vref = 5.0, uint8_t sampleCount = 30);

// Call once in your loop() to sample & update internal state
void update_tdsSensor();

// Get the last computed TDS (ppm)
float getValue_tdsSensor();

// If you measure water temperature elsewhere, call this:
void setTemp_tdsSensor(float celsius);

#endif // TDS_SENSOR_H