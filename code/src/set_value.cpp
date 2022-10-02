/**
 * @file SetValue.cpp
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

#include "set_value.hh"

/**
  * @brief initializes the object
 * 
 * @param digits number of all digits of the value
 * @param decimals number of digits after decimal point
 * @param min minimum value
 * @param max maximum value
 */
void SetValue::init(int digits, int decimals, float min, float max){
  numOfDigits = digits;
  numOfDecimalPlaces = decimals;
  minValue = min;
  maxValue = max;
  maxDecimalPlace = static_cast<DecimalPlaces> (digits - decimals -1);
  minDecimalPlace = static_cast<DecimalPlaces> (-decimals);
}

void SetValue::display(LiquidCrystal_I2C& lcd){
  char displayValue[20];
  int decimal = 0;
  if(numOfDecimalPlaces > 0)  //if value has decimal point
    decimal = 1;
  dtostrf(value, (numOfDigits + decimal), numOfDecimalPlaces, displayValue);
	lcd.print(displayValue);
}

/**
 * @brief clamps the value if it exceeds minimum and maximum value given via SetValue::init()
 * 
 */
void SetValue::limit(){
  if(value < minValue)
    value = minValue;
  else if(value > maxValue)
    value = maxValue;
}

/**
 * @brief decrementation operator overload  for DecimalPlaces enum
 * 
 * @param dec 
 */
void operator--(DecimalPlaces& dec){
  int tmp = static_cast<int>(dec);
  --tmp;
  dec = static_cast<DecimalPlaces>(tmp);
}