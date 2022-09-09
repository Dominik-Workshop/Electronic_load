/**
 * @file measurements.hh
 * @author Dominik Workshop
 * @brief takes voltage, current and temperature measurements,
 * calculates power, stores these values, can display all the measurements on the lcd
 * @version 1.0
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MEASUREMENTS_HH
#define MEASUREMENTS_HH

#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal_I2C.h>
#include <MCP79410_Timer.h>
#include <math.h>

#include "defines.hh"
#include "lcd_characters.hh"
#include "calibration.hh"

/**
 * @brief contains measured voltage, current, temperature, calculated power, calibration values and ADC object
 * 
 */
class Measurements{
  public:
    float voltage;
    float current;
    float power;
    int temperature;
    AdcCalibration calibration;
    Adafruit_ADS1115 adc;
    MCP79410_Timer timer = MCP79410_Timer(RTC_ADDRESS);

    Measurements();
    void update();
    void displayMeasurements(LiquidCrystal_I2C& lcd);
    float measureVoltage();
    float measureCurrent();
    int measureTemperature();
  private:
    void display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces);
    void noLessThanZero(float& value);
};

#endif