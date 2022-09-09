/**
 * @file calibration.cpp
 * @author Dominik Workshop
 * @brief methods for reading calibration values from EEPROM and writing them to EEPROM, getters and setters
 * @version 1.2
 * @date 2022-07-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <EEPROM.h>
#include "calibration.hh"

/**
 * @brief ensures that given value is in range from 0 to 255
 * (you can't write numbers outside this range to the EEPROM)
 * 
 * @param value to be limited
 * @return int 0-255
 */
int limit(int& value){
  if(value < 0)
    value = 0;
  else if(value > 255)
    value = 255;
  return value;
}

/**
 * @brief reads calibration values from EEPROM
 * 
 */
void AdcCalibration::readFromEEPROM(){
  voltageMultiplier = EEPROM.read(EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL);
  currentMultiplier = EEPROM.read(EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL);
  currentOffset = EEPROM.read(EEPROM_ADDRESS_ADC_I_OFFSET_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void AdcCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL, voltageMultiplier);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL, currentMultiplier);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_OFFSET_CAL, currentOffset);
}

int AdcCalibration::getVoltageMultiplier(){return voltageMultiplier;}
int AdcCalibration::getCurrentMultiplier(){return currentMultiplier;}
int AdcCalibration::getCurrentOffset(){return currentOffset;}

void AdcCalibration::setVoltageMultiplier(int cal){
  voltageMultiplier = limit(cal);
}
void AdcCalibration::setCurrentMultiplier(int cal){
  currentMultiplier = limit(cal);
}
void AdcCalibration::setCurrentOffset(int cal){
  currentOffset = limit(cal);
}

/**
 * @brief reads calibration values from EEPROM
 * 
 */
void DacCalibration::readFromEEPROM(){
  setCurrentMultiplier = EEPROM.read(EEPROM_ADDRESS_DAC_MULTIPLIER_CAL);
  setCurrentOffset = EEPROM.read(EEPROM_ADDRESS_DAC_OFFSET_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void DacCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_DAC_MULTIPLIER_CAL, setCurrentMultiplier);
  EEPROM.write(EEPROM_ADDRESS_DAC_OFFSET_CAL, setCurrentOffset);
}

int DacCalibration::getSetCurrentMultiplier(){return setCurrentMultiplier;}
int DacCalibration::getSetCurrentOffset(){return setCurrentOffset;}

void DacCalibration::setSetCurrentMultiplier(int cal){
  setCurrentMultiplier = limit(cal);
}
void DacCalibration::setSetCurrentOffset(int cal){
  setCurrentOffset = limit(cal);
}