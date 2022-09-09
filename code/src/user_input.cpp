/**
 * @file user_input.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 1.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "user_input.hh"

UserInput::UserInput(){
  setCurrent.value = 0.1; //initialize set current to 0,1A at power up
  setCurrent.init(4, 3, 0.004, MAX_CURRENT);
  setPower.value = 1;  //initialize set power to 1W at power up
  setPower.init(5,2, 0.01, MAX_POWER);
  setResistance.value = 100;  //initialize set resitance to 100ohm at power up
  setResistance.init(5,1, 0.1, MAX_RESISTANCE);
}