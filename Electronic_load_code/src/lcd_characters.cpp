/**
 * @file lcd_characters.cpp
 * @author Dominik Workshop
 * @brief definition of arrays for custom character's pixels
 * @version 0.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "lcd_characters.hh"

/**
 * @brief array of pixels for degree symbol
 * 
 */
uint8_t degreeSymbol[] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

/**
 * @brief array of pixels for ohm symbol
 * 
 */
uint8_t ohmSymbol[] = {
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B01010,
  B11011,
  B00000
};
