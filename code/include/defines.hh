/**
 * @file defines.hh
 * @author Dominik Workshop
 * @brief Addresses of I2C devices, addresses of calibration values in EEPROM and names of used pins, other constants
 * @version 1.1
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

#define EEPROM_ADDRESS_DAC_MULTIPLIER_CAL 0       //address for calibration value of DAC - setCurrentMultiplier
#define EEPROM_ADDRESS_DAC_OFFSET_CAL 1           //address for calibration value of DAC - setCurrentOffset
#define EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL 2     //address for calibration value of ADC - voltageMultiplier
#define EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL 3     //address for calibration value of ADC - currentMultiplier
#define EEPROM_ADDRESS_ADC_I_OFFSET_CAL 4         //address for calibration value of ADC - currentOffset

#define KEYPAD_ROWS 4   //number of rows of the keypad
#define KEYPAD_COLS 4   //number of collumns of the keypad
#define KP1 6           //Keypad pin
#define KP2 7           //Keypad pin
#define KP3 8           //Keypad pin
#define KP4 9           //Keypad pin
#define KP5 10          //Keypad pin
#define KP6 14          //Keypad pin
#define KP7 15          //Keypad pin
#define KP8 16          //Keypad pin
#define KP9 11          //Keypad pin

#define ENCODER_A 2
#define ENCODER_B 17
#define ENCODER_BUTTON 3

#define THERMISTOR A6
#define R1 10000
#define c1 1.009249522e-03
#define c2 2.378405444e-04
#define c3 2.019202697e-07

#define FAN 5
#define OUTPUT_OFF 4
#define IN_TRIGGER 3

#endif