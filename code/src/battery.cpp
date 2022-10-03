/**
 * @file battery.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-09-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "battery.hh"

Battery::Battery(){
  cutoffVoltage.value = 0;
  cutoffVoltage.init(4, 2, 0, 50);
  dischargeCurrent.value = 0.01;
  dischargeCurrent.init(4, 3, 0.004, MAX_CURRENT);
}

/**
 * @brief displays battery capacity in mAh, in the top left corner of the lcd
 * 
 * @param lcd 
 */
void Battery::displayCapacity(LiquidCrystal_I2C& lcd){
  char displayValue[20];
  dtostrf(capacity, 5, 0, displayValue);
  lcd.setCursor(0, 0);
	lcd.print(displayValue);
  lcd.print("mAh");
}