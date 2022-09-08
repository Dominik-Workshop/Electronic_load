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