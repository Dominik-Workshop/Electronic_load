/**
 * @file mode_screen.hh
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MODE_SCREEN_HH
#define MODE_SCREEN_HH

#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP4725.h>
#include "lcd_characters.hh"
#include "user_input.hh"
#include "encoder.hh"
#include "keypad_config.hh"
#include "temperature.hh"



void welcomeScreen(LiquidCrystal_I2C& lcd);
void mainMenu(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac);
void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac);
void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac);
void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac);
void transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac);
void batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac);

int inputFromKeypad(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, SetValue& setValue);
void displayTemperature(LiquidCrystal_I2C& lcd);

#endif

/*
  A = 10
  B = 11
  C = 12
  D = 13
  E = 14
  F = 15
  G = 16
  H = 17
  I = 18
  J = 19

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | C o n s t   C u r r e n t         O f f |
1 | 0 0 . 0 0 0 V   0 . 0 0 0 A   0 . 0 0 W |
2 | S e t   I = 0 . 0 0 0 A                 |
3 |                                 2 3 * C |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | C o n s t   P o w e r             O f f |
1 | 0 0 . 0 0 0 V   0 . 0 0 0 A   0 . 0 0 W |
2 | S e t   P = 0 0 0 . 0 0 W               |
3 |                                 2 3 * C |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | C o n s t   R e s i s t a n c e   O f f |
1 | 0 0 . 0 0 0 V   0 . 0 0 0 A   0 . 0 0 W |
2 | S e t   R = 0 0 0 0 . 0 Ω               |
3 |                                 2 3 * C |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | 1 . C o n s t .   C u r r e n t         |
1 | 2 . C o n s t .   P o w e r             |
2 | 3 . C o n s t .   R e s i s t a n c e   |
3 | 4 . T r a n s i e n t     5 . B a t t . |
  -------------------------------------------
*/