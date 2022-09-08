/**
 * @file calibration.hh
 * @author Dominik Workshop
 * @brief class containing calibration values for the DAC and ADC
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
 * @brief contains calibration values for the ADC
 * 
 */
class AdcCalibration{
  public:
    int current;       //measured current - calibration multiplier 0-255
    int voltage;       //measured voltage - calibration multiplier 0-255
    void readFromEEPROM();
    void writeToEEPROM();
};

/**
 * @brief contains calibration values for the DAC
 * 
 */
class DacCalibration{
  public:
    int dac;               //set current - calibration multiplier 0-255
    void readFromEEPROM();
    void writeToEEPROM();
};

#endif