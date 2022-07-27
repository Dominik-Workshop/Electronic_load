/**
 * @file calibration.hh
 * @author Dominik Workshop
 * @brief calss containing calibration values for the DAC and ADC
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CALIBRATION_HH
#define CALIBRATION_HH

#include "defines.hh"

/**
 * @brief contains calibration values for the DAC and ADC
 * 
 */
class CalibrationValues{
  public:
    int DAC;               //set current - calibration multiplier
    int ADC_current;       //measured current - calibration multiplier
    int ADC_voltage;       //measured voltage - calibration multiplier
    void readFromEEPROM();
    void save(int dac, int adc_i, int adc_u);
    void writeToEEPROM();
};

#endif