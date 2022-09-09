/**
 * @file calibration.hh
 * @author Dominik Workshop
 * @brief classes containing calibration values for the ADC and DAC
 * @version 1.2
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
};

/**
 * @brief contains calibration values for the DAC
 * 
 */
class DacCalibration{
  public:
    void readFromEEPROM();
    void writeToEEPROM();

    int getSetCurrentMultiplier();
    int getSetCurrentOffset();

    void setSetCurrentMultiplier(int cal);
    void setSetCurrentOffset(int cal);

  private:
    int setCurrentMultiplier;    //set current - calibration multiplier 0-255
    int setCurrentOffset;        //set current - calibration offset     0-255
};

#endif