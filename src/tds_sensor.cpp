#include "tds_sensor.h"

// configuration & buffers
static uint8_t  _pin;
static float    _vref;
static uint8_t  _sampleCount;
static int    * _analogBuffer;
static int    * _analogBufferTemp;
static uint8_t  _analogBufferIndex;
static float    _temperature;
static float    _lastTds;




//* Initalize all static values/arrays
void init_tdsSensor(uint8_t analogPin, float VREF, uint8_t sampleCount){
  _pin         = analogPin;
  _vref        = VREF;
  _sampleCount       = sampleCount;
  _analogBuffer      = (int*)malloc(_sampleCount * sizeof(int));
  _analogBufferTemp  = (int*)malloc(_sampleCount * sizeof(int));
  _analogBufferIndex = 0;
  _temperature = 25.0;
  pinMode(_pin, INPUT);
}

//*set temp based off outside reading
void setTemp_tdsSensor(float temp) {
  _temperature = temp;
}

void update_tdsSensor() {
  //* Set Current Timestamp
  static unsigned long analogSampleTimepoint = millis();
  static unsigned long analog30SampleTimepoint = millis();
  
  //every 40 milliseconds,take ONE sample analog value from the ADC-sensor
  if(millis()-analogSampleTimepoint > 40U) {
      analogSampleTimepoint = millis();
      _analogBuffer[_analogBufferIndex] = analogRead(_pin);
      if(_analogBufferIndex == _sampleCount) //*reset index
          _analogBufferIndex = 0;
    }   

  //every 40 milliseconds,read the analog value from the ADC  
  if (millis() - analog30SampleTimepoint > 800U){
    analog30SampleTimepoint = millis();

    // Copy buffer for sorting
    for (int i = 0; i < _sampleCount; i++)
    _analogBufferTemp[i] = _analogBuffer[i];  

    // 1) Median filter → removes spikes
    int medianReading = getMedianNum(_analogBufferTemp, _sampleCount);

    // 2) Convert to voltage
    float averageVoltage = medianReading * _vref / 1024.0;
    
    // 3) Temperature compensation
    float compensationCoefficient = 1.0 + 0.02 * (_temperature - 25);
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // 4) TDS conversion (empirical cubic formula) and scaling
    _lastTds = (133.42 * pow(compensationVoltage, 3)
    - 255.86 * pow(compensationVoltage, 2)
    + 857.39 * compensationVoltage) * 0.5; 
    }
}

float getValue_tdsSensor() {
  return _lastTds;
}


//* getMedianNum Function within Scope
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
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
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}