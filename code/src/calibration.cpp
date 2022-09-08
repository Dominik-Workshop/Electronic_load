/**
 * @file calibration.cpp
 * @author Dominik Workshop
 * @brief methods for storing calibration values temporarily, reading them from EEPROM and writing them to EEPROM
 * @version 0.1
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
  current = EEPROM.read(EEPROM_ADDRESS_ADC_I_CAL);
  voltage = EEPROM.read(EEPROM_ADDRESS_ADC_U_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void AdcCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_ADC_I_CAL, current);
  EEPROM.write(EEPROM_ADDRESS_ADC_U_CAL, voltage);
}


/**
 * @brief reads calibration values from EEPROM
 * 
 */
void DacCalibration::readFromEEPROM(){
  dac = EEPROM.read(EEPROM_ADDRESS_DAC_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void DacCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_DAC_CAL, dac);
}