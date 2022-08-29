/**
 * @file keypad.hh
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef KEYPAD_HH
#define KEYPAD_HH

#include <Keypad.h>
#include "defines.hh"

enum SpecialKeys{
    LoadOnOff,
    Menu,
    Enter,
    Delete
};

extern char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS];
extern uint8_t rowPins[KEYPAD_ROWS];
extern uint8_t colPins[KEYPAD_COLS];

#endif