/**
 * @file calibration.hh
 * @author Dominik Workshop
 * @brief class containing calibration values for the DAC and ADC
 * @version 1.1
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
    void readFromEEPROM();
    void writeToEEPROM();

    int getVoltageMultiplier();
    int getCurrentMultiplier();
    int getCurrentOffset();

    void setVoltageMultiplier(int cal);
    void setCurrentMultiplier(int cal);
    void setCurrentOffset(int cal);

  private:
    int voltageMultiplier;       //measured voltage - calibration multiplier 0-255
    int currentMultiplier;       //measured current - calibration multiplier 0-255
    int currentOffset;           //measured current - calibration offset     0-255
    int limit(int& value);
};

/**
 * @brief contains calibration values for the DAC
 * 
 */
class DacCalibration{
  public:
    int dacMultiplier;               //set current - calibration multiplier 0-255
    void readFromEEPROM();
    void writeToEEPROM();
};

#endif