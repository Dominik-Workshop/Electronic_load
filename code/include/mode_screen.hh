/**
 * @file mode_screen.hh
 * @author Dominik Workshop
 * @brief 
 * @version 2.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MODE_SCREEN_HH
#define MODE_SCREEN_HH


#include <LiquidCrystal_I2C.h>

#include "lcd_characters.hh"
#include "user_input.hh"
#include "encoder.hh"
#include "keypad_config.hh"
#include "measurements.hh"
#include "controls.hh"
#include "battery.hh"

enum ModeOfOperation{
  ConstCurrent,
  ConstPower,
  ConstResistance,
  Calibration
};

void displayWelcomeScreen(LiquidCrystal_I2C& lcd);
void displayMenu(LiquidCrystal_I2C& lcd);

void mainMenu(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
void transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
void batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);

void taskLoop(ModeOfOperation mode, SetValue& setParameter, LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
void loadControl(Controls& controls, UserInput& userInput, ModeOfOperation mode);


void calibration(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);

#endif

/*
    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | 1 . C o n s t .   C u r r e n t         |
1 | 2 . C o n s t .   P o w e r             |
2 | 3 . C o n s t .   R e s i s t a n c e   |
3 | 4 . T r a n s i e n t     5 . B a t t . |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | C o n s t   C u r r e n t         O F F |
1 | 0 . 0 0 0 V   0 . 0 0 0 A     0 . 0 0 W |
2 | S e t   I = 0 . 1 0 0 A                 |
3 |                                 2 3 * C |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | C o n s t   P o w e r             O F F |
1 | 0 . 0 0 0 V   0 . 0 0 0 A     0 . 0 0 W |
2 | S e t   P =     1 . 0 0 W               |
3 |                                 2 3 * C |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | C o n s t   R e s i s t a n c e   O F F |
1 | 0 . 0 0 0 V   0 . 0 0 0 A     0 . 0 0 W |
2 | S e t   R =   1 0 0 . 0 Ω               |
3 |                                 2 3 * C |
  -------------------------------------------

    0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J
  -------------------------------------------
0 | 0 0 0 0 0 m A h                   O F F |
1 | 0 . 0 0 0 V   0 . 0 0 0 A     0 . 0 0 W |
2 | I = 0 . 1 0 0 A     O f f =   0 . 0 0 V |
3 |               0 0 : 0 0 : 0 0   2 3 * C |
  -------------------------------------------
*/