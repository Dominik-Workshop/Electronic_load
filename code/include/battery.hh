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

/**
 * @brief contains battery capacity in mAh, discharge time, setValues: cutoffVoltage and dischargeCurrent,
 * information which setValue is currently changed
 * 
 */
class Battery{
  public:
    float capacity = 0;         //in mAh
    uint32_t dischargeTime = 0; //in seconds
    SetValue cutoffVoltage;     //in Volts
    SetValue dischargeCurrent;  //in Amps
    BatteryChangedVariable changedVariable;
    
    Battery();
    void displayCapacity(LiquidCrystal_I2C& lcd);
  private:
    
};

#endif