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
  cutoffVoltage.init(3, 2, 0, 50);
}

void Battery::displaycutoffVoltage(LiquidCrystal_I2C& lcd){
  lcd.print(cutoffVoltage.value, 2);
}