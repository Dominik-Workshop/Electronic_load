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
void CalibrationValues::readFromEEPROM(){
  DAC = EEPROM.read(EEPROM_ADDRESS_DAC_CAL);
  ADC_current = EEPROM.read(EEPROM_ADDRESS_ADC_I_CAL);
  ADC_voltage = EEPROM.read(EEPROM_ADDRESS_ADC_U_CAL);
}

/**
 * @brief saves current calibration values in EEPROM
 * 
 */
void CalibrationValues::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_DAC_CAL, DAC);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_CAL, ADC_current);
  EEPROM.write(EEPROM_ADDRESS_ADC_U_CAL, ADC_voltage);
}

/**
 * @brief saves new calibration values until power down,
 * to store it permanently use "writeToEEPROM" method
 * 
 * @param dac new calibration value for the DAC
 * @param adc_i new calibration value for the ADC_current
 * @param adc_u new calibration value for the ADC_voltage
 */
void CalibrationValues::save(int dac, int adc_i, int adc_u){
  DAC = dac;
  ADC_current = adc_i;
  ADC_voltage = adc_u;
}