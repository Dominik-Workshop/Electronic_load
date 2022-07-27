/**
 * @file defines.hh
 * @author Dominik Workshop
 * @brief Addresses of I2C devices, addresses of calibration values in EEPROM and names of used pins
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DEFINES_HH
#define DEFINES_HH

#define ADC_ADDRESS 0x48
#define DAC_ADDRESS 0x60
#define LCD_ADDRESS 0x27
#define RTC_ADDRESS 0x6f

#define EEPROM_ADDRESS_DAC_CAL 0    //address for calibration value of DAC
#define EEPROM_ADDRESS_ADC_I_CAL 1  //address for calibration value of ADC - current
#define EEPROM_ADDRESS_ADC_U_CAL 2  //address for calibration value of ADC - voltage

#define KP1 6   //Keypad pin
#define KP2 7   //Keypad pin
#define KP3 8   //Keypad pin
#define KP4 9   //Keypad pin
#define KP5 10  //Keypad pin
#define KP6 11  //Keypad pin
#define KP7 12  //Keypad pin

#define TEMPERATURE A6
#define FAN 5
#define OUTPUT_OFF 4
#define IN_TRIGGER 3

#endif