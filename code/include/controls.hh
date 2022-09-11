/**
 * @file controls.hh
 * @author Dominik Workshop
 * @brief fan, load on/off, current/power/resitance regulation controlls
 * @version 1.1
 * @date 2022-09-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CONTROLS_HH
#define CONTROLS_HH

#include <Adafruit_MCP4725.h>
#include "measurements.hh"
#include "calibration.hh"
#include "defines.hh"

/**
 * @brief contains DAC object and calibration values, has acces to measurements, has methods for controlling the load
 * 
 */
class Controls{
  public:
    Adafruit_MCP4725 dac;
    DacCalibration calibration;
    Measurements& measurements;   //copy of measurements
    void fanControll();
    void loadOn(LiquidCrystal_I2C& lcd);
    void loadOff(LiquidCrystal_I2C& lcd);
    void loadOnOffToggle(LiquidCrystal_I2C& lcd);
    void regulateCurrent(float setCurrent);
    void regulatePower(float setPower);
    void regulateResistance(float setResistance);
    bool isLoadOn();

    Controls(Measurements& measurements_);
  private:
    int fanspeed;
    int lowTemperatureThreshold = 30;   //fan turns on after reaching this temperature
    int highTemperatureThreshold = 45;  //fan reaches full speed at this temperature
    bool loadIsOn = false;
};


#endif