/**
 * @file controls.hh
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
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

class Controls{
  public:
    Adafruit_MCP4725 dac;
    DacCalibration calibration;
    void fanControll(Measurements& measurements);
    void loadOn(LiquidCrystal_I2C& lcd);
    void loadOff(LiquidCrystal_I2C& lcd);
    void loadOnOffToggle(LiquidCrystal_I2C& lcd);
    void sinkCurrent(float setCurrent);
    void drawConstPower(float setPower, float voltage);
    void constResistance(float setResistance, float voltage);

    Controls();
  private:
    int fanspeed;
    int lowTemperatureThreshold = 30;   //fan turns on after reaching this temperature
    int highTemperatureThreshold = 45;  //fan reaches full speed at this temperature
    bool loadIsOn = false;
};


#endif