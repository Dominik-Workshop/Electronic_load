/**
 * @file measurements.hh
 * @author Dominik Workshop
 * @brief takes voltage and current measurements, calculates power, stores these values, can display all the measurements on the lcd
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

/**
 * @brief contains measured voltage, current and calculated power
 * 
 */
class Measurements{
  public:
    float voltage;
    float current;
    float power;

    void update(Adafruit_ADS1115& adc);
    void displayMeasurements(LiquidCrystal_I2C& lcd);
  private:
    void display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces);
    void noLessThanZero(float& value);
};

#endif