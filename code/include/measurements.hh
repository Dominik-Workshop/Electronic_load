/**
 * @file measurements.hh
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MEASUREMENTS_HH
#define MEASUREMENTS_HH

#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal_I2C.h>

class Measurements{
  public:
    float voltage;
    float current;
    float power;

    void update(Adafruit_ADS1115& adc);
    void displayMeasurements(LiquidCrystal_I2C& lcd);
  private:
    void display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces);
};

#endif