/**
 * @file battery.hh
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-09-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BATTERY_HH
#define BATTERY_HH

#include <LiquidCrystal_I2C.h>

#include "set_value.hh"

class Battery{
  public:
    SetValue cutoffVoltage; //in Volts
    int capacity = 0;       //in mAh
    
    Battery();
    void displaycutoffVoltage(LiquidCrystal_I2C& lcd);
  private:
    
};

#endif