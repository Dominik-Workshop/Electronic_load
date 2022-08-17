/**
 * @file user_input.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "user_input.hh"

UserInput::UserInput(){
  setPower.value = 1;  //initialize set power to 1W at power up
  setPower.init(5,2, 0.01, 200.00);
  setResistance.value = 100;  //initialize set resitance to 100ohm at power up
  setResistance.init(5,1, 0.1, 9000.0);
}