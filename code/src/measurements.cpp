/**
 * @file measurements.cpp
 * @author Dominik Workshop
 * @brief takes voltage and current measurements, calculates power, stores these values, can display all the measurements on the lcd
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "measurements.hh"

/**
 * @brief make votage and current readings
 * 
 * @param adc 
 */
void Measurements::update(Adafruit_ADS1115& adc){
            //        reading_from_adc / resolution * Vmax * attenuation
  voltage = (adc.readADC_Differential_2_3()/ 32768.0) * 0.256 * 100;
  noLessThanZero(voltage);
            //      reading_from_adc / resolution * Vmax / R  * number of shunts
  current = ((adc.readADC_SingleEnded(0) / 32768.0) * 0.256 / 0.1) * 4;
  noLessThanZero(current);
  power = voltage * current;
}

void Measurements::displayMeasurements(LiquidCrystal_I2C& lcd){
  power = voltage * current;
  lcd.setCursor(0,1);
  display(lcd, voltage, 5, 2);
  lcd.print("V ");
  display(lcd, current, 4, 3);
  lcd.print("A ");
  display(lcd, power, 3, 1);
  lcd.print("W");
}

/**
 * @brief displays given number with specified amount of digits and decimal places
 * 
 * @param lcd 
 * @param value to be displayed
 * @param numOfDigits 
 * @param numOfDecimalPlaces 
 */
void Measurements::display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces){
  char displayValue[10];
  dtostrf(value, (numOfDigits + 1), numOfDecimalPlaces, displayValue);
	lcd.print(displayValue);
}

/**
 * @brief if value is negative it becomes zero
 * 
 * @param value is limited to be only positive
 */
void Measurements::noLessThanZero(float& value){
  if(value < 0)
    value = 0;
}