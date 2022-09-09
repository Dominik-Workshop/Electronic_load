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

void fanControll(Measurements& measurements);
void loadOnOffToggle(LiquidCrystal_I2C& lcd);
void sinkCurrent(float current, Adafruit_MCP4725& dac);

#endif