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

/**
 * @brief used for storing data recieved from the user via keypad or rotary encoder
 * 
 */
class UserInput{
  public:
    SetValue setCurrent;
    SetValue setPower;
    SetValue setResistance;
    char key = ' ';                     //stores one character entered from keypad
    DecimalPlaces decimalPlace = ones;  //stores information where the encoder's cursor is in terms of decimal place of setValue  
    int cursorPos = 7;                  //cursor's position on the lcd
		uint32_t time;							        //used to measure time from last action
    UserInput();
};

#endif