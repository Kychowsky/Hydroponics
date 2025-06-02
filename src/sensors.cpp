#include "sensors.h"

// --- Internal Sensor Instances ---
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);
ADS1115 ads(0x48);  // Default I2C address for ADS1115

// --- Sensor Initialization ---
void init_sensors() {
  Wire.begin(D2, D1);  // ESP8266 I2C pins: SDA = D2 (GPIO4), SCL = D1 (GPIO5)
  sensors.begin();
  ads.setGain(ADS1X15_GAIN_6144MV);  // Gain to read 0–6.144V range

  if (!ads.begin()) {
    Serial.println("ADS1115 not found!");  // You can forward this to web output if needed
  } else {
    Serial.println("ADS1115 found!");
  }
}

// --- Raw ADC Read Functions ---
int readAdsPH() {
  return ads.readADC(PH_ADS_CHANNEL);
}

int readAdsTDS() {
  return ads.readADC(TDS_ADS_CHANNEL);
}

// --- Averaging Function Using Function Pointer ---
int getAveragedADC(int (*readFunc)()) {
  int buffer[SAMPLE_COUNT];
  static unsigned long lastSampleTime = 0;
  uint8_t index = 0;

  while (index < SAMPLE_COUNT) {
    if (millis() - lastSampleTime >= 40) {
      lastSampleTime = millis();
      buffer[index++] = readFunc();
    }
    yield();  // Prevent watchdog reset (important for ESP8266)
  }

  return getMedianNum(buffer, SAMPLE_COUNT);
}

// --- Median Filter ---
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++) {
    bTab[i] = bArray[i];
  }

  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }

  if ((iFilterLen & 1) > 0)
    return bTab[(iFilterLen - 1) / 2];
  else
    return (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
}

// --- Temperature Sensor ---
float readTempC() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// --- pH Calculation ---
float readPH() {
  int averagedRaw = getAveragedADC(readAdsPH);
  float voltage = averagedRaw * gainMultiplier;
  return voltage * m + C;
}

// --- TDS Calculation ---
float readTDS() {
  int averagedRaw = getAveragedADC(readAdsTDS);
  float voltage = averagedRaw * gainMultiplier;

  float temp = readTempC();
  float compensation = 1.0 + 0.02 * (temp - 25.0);
  float compensatedVoltage = voltage / compensation;

  return (133.42 * pow(compensatedVoltage, 3)
        - 255.86 * pow(compensatedVoltage, 2)
        + 857.39 * compensatedVoltage) * 0.5;
}