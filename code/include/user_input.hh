/**
 * @file user_input.hh
 * @author Dominik Workshop
 * @brief class containing data recieved from the user via keypad or rotary encoder
 * @version 0.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef USER_INPUT_HH
#define USER_INPUT_HH

#include <Arduino.h>
#include "set_value.hh"

enum DecimalPlaces{
  thousandths = -3, //0.01
  hundredths,       //0.01
  tenths,           //0.1
  ones,             //1
  tens,             //10
  hundreds,         //100
  thousands         //1000
};

/**
 * @brief used for storing data recieved from the user via keypad or rotary encoder
 * 
 */
class UserInput{
  public:
    SetValue setCurrent;
    SetValue setPower;
    SetValue setResistance;
    char key = ' ';               //stores one character entered from keypad
    float decimalPlace = ones;       //stores information where the encoder's cursor is
    int cursorPos = 7;
		uint32_t time;							  //used to measure time from last action
    UserInput();
};

#endif