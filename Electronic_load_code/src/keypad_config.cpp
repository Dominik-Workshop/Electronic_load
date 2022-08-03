/**
 * @file keypad.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "keypad_config.hh"

//define the symbols on the buttons of the keypads
char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// uint8_t rowPins[KEYPAD_ROWS] = {KP1, KP2, KP3, KP4}; //connect to the row pin outs of the keypad
// uint8_t colPins[KEYPAD_COLS] = {KP5, KP6, KP7, KP8}; //connect to the column pin outs of the keypad
uint8_t rowPins[KEYPAD_ROWS] = {12, 11, 10, 9}; //connect to the row pin outs of the keypad
uint8_t colPins[KEYPAD_COLS] = {8, 7, 6, 5}; //connect to the column pin outs of the keypad