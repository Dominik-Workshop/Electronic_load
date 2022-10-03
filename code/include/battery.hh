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
#include "defines.hh"

enum BatteryChangedVariable{
  DischargeCurrent,
  CutoffVoltage
};

class Battery{
  public:
    SetValue cutoffVoltage; //in Volts
    SetValue dischargeCurrent;
    float capacity = 0;       //in mAh
    uint32_t dischargeTime = 0;
    BatteryChangedVariable changedVariable;
    
    Battery();
    void displayCapacity(LiquidCrystal_I2C& lcd);
  private:
    
};

#endif