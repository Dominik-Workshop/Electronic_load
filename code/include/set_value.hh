/**
 * @file setValue.hh
 * @author Dominik Workshop
 * @brief class which can:
 *  - store a float value, 
 *  - display it with specified amount of digits on an lcd in such a way that it will always take up the same amount of space
 *  - limit the value to specified range
 * @version 0.1
 * @date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SET_VALUE_HH
#define SET_VALUE_HH

#include <LiquidCrystal_I2C.h>

class SetValue{
  public:
    float value;
    void init(int digits, int decimals, float min, float max);
    void display(LiquidCrystal_I2C& lcd);
    void limit();
  private:
    int numOfDigits;
    int numOfDecimalPlaces;
    float maxValue;
    float minValue;
};

#endif