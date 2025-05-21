  #include <nutrients_pump.h>
  
void nutrient_injection(int PPM, GrowthStage stage){
  
    // Nutrient Dosing based on PPM
    // PPM Chart
    // Seedling: 100–300 ppm - Use 1/4 strength Flora Micro & Flora Bloom only (no Grow)
    // Early Vegetative: 400–600 ppm - Light feed: Flora Micro 2.5ml, Grow 2.5ml, Bloom 1ml per gallon
    // Mid Vegetative: 600–800 ppm - Increase dose: Micro 5ml, Grow 5ml, Bloom 2ml per gallon
    // Late Vegetative: 800–1000 ppm - Full veg strength: Micro 6ml, Grow 9ml, Bloom 3ml per gallon
    // Early Flowering: 1000–1200 ppm - Transition: Micro 6ml, Grow 4ml, Bloom 6ml per gallon
    // Mid Flowering: 1200–1400 ppm - Peak bloom: Micro 6ml, Grow 2ml, Bloom 10ml per gallon
    // Late Flowering: 1000–1200 ppm - Maintain bloom mix, slowly taper Grow
    // Final Week (Flush): 0–200 ppm - Use only pH-balanced water, no nutrients
int targetMinPPM = 0;
int targetMaxPPM = 0;

switch (stage) {
  case SEEDLING:
    targetMinPPM = 100;
    targetMaxPPM = 300;
    if (PPM < 100){
        //start pumps
    }
    break;
  case EARLY_VEG:
    targetMinPPM = 400;
    targetMaxPPM = 600;
        if (PPM < 100){
        //start pumps
    }
    break;
  case MID_VEG:
    targetMinPPM = 600;
    targetMaxPPM = 800;
    if (PPM < 100){
        //start pumps
    }    
    break;
  case LATE_VEG:
    targetMinPPM = 800;
    targetMaxPPM = 1000;
    if (PPM < 100){
        //start pumps
    }
    break;
  case EARLY_FLOWER:
    targetMinPPM = 1000;
    targetMaxPPM = 1200;
    break;
  case MID_FLOWER:
    targetMinPPM = 1200;
    targetMaxPPM = 1400;
    break;
  case LATE_FLOWER:
    targetMinPPM = 1000;
    targetMaxPPM = 1200;
    break;
  case FLUSH:
    targetMinPPM = 0;
    targetMaxPPM = 200;
    break;
}
}

