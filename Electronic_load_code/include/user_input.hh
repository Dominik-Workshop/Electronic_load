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

/**
 * @brief used for storing data recieved from the user via keypad or rotary encoder
 * 
 */
class UserInput{
  public:
    float setCurrent = 0;
    float setPower = 0;
    float setResistance = 0;
    char numbers[10];						//array of characters that will be converted to float later
		int index = 0;							//index for array numbers[]
    char key = ' '; 
		bool decimalPoint = false;	//indicates if user already input a decimal point
		uint32_t time;							//used to measure time from last action
};

#endif