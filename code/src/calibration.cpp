/**
 * @file calibration.cpp
 * @author Dominik Workshop
 * @brief methods for reading calibration values from EEPROM and writing them to EEPROM, getters and setters
 * @version 1.1
 * @date 2022-07-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <EEPROM.h>
#include "calibration.hh"

/**
 * @brief reads calibration values from EEPROM
 * 
 */
void AdcCalibration::readFromEEPROM(){
  voltageMultiplier = EEPROM.read(EEPROM_ADDRESS_ADC_U_CAL);
  currentMultiplier = EEPROM.read(EEPROM_ADDRESS_ADC_I_CAL);
  currentOffset = EEPROM.read(EEPROM_ADDRESS_ADC_I_OFFSET_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void AdcCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_ADC_U_CAL, voltageMultiplier);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_CAL, currentMultiplier);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_OFFSET_CAL, currentOffset);
}

/**
 * @brief reads calibration values from EEPROM
 * 
 */
void DacCalibration::readFromEEPROM(){
  dacMultiplier = EEPROM.read(EEPROM_ADDRESS_DAC_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void DacCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_DAC_CAL, dacMultiplier);
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
 * @brief ensures that given value is in range from 0 to 255
 * 
 * @param value to be limited
 * @return int 0-255
 */
int AdcCalibration::limit(int& value){
  if(value < 0)
    value = 0;
  else if(value > 255)
    value = 255;
  return value;
}