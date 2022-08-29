/**
 * @file lcd_characters.hh
 * @author Dominik Workshop
 * @brief custom characters for the LCD
 *  enum Symbols - contains names of symbols that will be used when referring to a given character
 *  uint8_t [] - array of pixels for given character
 * @version 0.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LCD_CHARACTERS_HH
#define LCD_CHARACTERS_HH

#include <Arduino.h>

enum Symbols{
  degree,
  ohm
};

extern uint8_t degreeSymbol[];
extern uint8_t ohmSymbol[];

#endif