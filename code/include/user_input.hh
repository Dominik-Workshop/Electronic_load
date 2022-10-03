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
#include "defines.hh"
#include "keypad_config.hh"
#include "encoder.hh"

/**
 * @brief used for storing data recieved from the user via keypad and rotary encoder,
 * can manage input from keypad and encoder, contains setValues: setCurrent, setPower, setResistance,
 * one character entered from keypad, encoder's cursor position, time from last action
 * 
 */
class UserInput{
  public:
    SetValue setCurrent;
    SetValue setPower;
    SetValue setResistance;

    char key = ' ';       //stores one character entered from keypad
    void inputFromKeypad(LiquidCrystal_I2C& lcd, SetValue& setParameter, int xPositon, int yPosition);
    void resetKeypadInput();
    
    void checkEncoder(LiquidCrystal_I2C& lcd, SetValue& setParameter, Encoder& encoder, int xPositon, int yPosition);

    DecimalPlaces decimalPlace = ones;  //encoder's cursor position is in terms of decimal place of setValue  
    int cursorPosX = 7;                  //encoder's cursor position horizontally on the lcd
    int cursorPosY = 2;                 //encoder's cursor position vertically on the lcd

		uint32_t time;							        //used to measure time from last action

    UserInput();
  private:
    char numbers[7] = {'\0', '0','0','0','0','0'};	//array of characters that will be converted to float later
    bool decimalPointPresent = false;								//indicates if user already input a decimal point
    int index = 0;							                    //index for array numbers[]
	  int x_pos = 0;                                  //cursor position for numbers entered via keypad
};

#endif