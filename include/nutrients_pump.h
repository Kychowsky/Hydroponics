#include <Arduino.h>

enum GrowthStage {
  SEEDLING = 0,
  EARLY_VEG,
  MID_VEG,
  LATE_VEG,
  EARLY_FLOWER,
  MID_FLOWER,
  LATE_FLOWER,
  FLUSH
};


void nutrient_injection(int PPM, GrowthStage stage){

}