/**
 * @file measurements.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "measurements.hh"

void limit(float& value){
  if(value < 0){
    value = 0;
  }
}

void Measurements::update(Adafruit_ADS1115& adc){
            //        reading_from_adc / resolution * Vmax * attenuation
  voltage = (adc.readADC_Differential_2_3()/ 65536.0) * 0.256 * 100 * 2;
  limit(voltage);
            //      reading_from_adc / resolution * Vmax / R  * number of shunts
  current = ((adc.readADC_SingleEnded(0) / 65536.0) * 0.256 / 0.1) * 4 * 2;
  limit(current);
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

void Measurements::display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces){
  char displayValue[10];
  dtostrf(value, (numOfDigits + 1), numOfDecimalPlaces, displayValue);
	lcd.print(displayValue);
}