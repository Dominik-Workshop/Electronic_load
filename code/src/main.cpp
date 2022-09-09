/**
 * @file main.cpp
 * @author Dominik Workshop
 * @brief Electronic load with 5 different modes of operation:
 *  - Constant current      0,001A - 9,999A,
 *  - Constant power        0,01W - 200,00W,
 *  - Constant resistance   0,1Ω - 9 999Ω,
 *  - Transient response
 *  - Battery capacity measurement
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP4725.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
//#include <MCP79410_Timer.h>

#include "defines.hh"
#include "init.hh"
#include "encoder.hh"
#include "keypad_config.hh"
#include "lcd_characters.hh"
#include "mode_screen.hh"
#include "user_input.hh"
#include "measurements.hh"
#include "controls.hh"


void setup() {
  Measurements measurements;
  Controls controls;
  UserInput userInput;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
  Encoder encoder;

  //TCCR0B = TCCR0B & B11111000 | B00000001; // PWM 31372.55 Hz pins 5 and 6

  lcd.init();
  lcd.backlight();              //turn the backlight on
  lcd.createChar(degree, degreeSymbol);
  lcd.createChar(ohm, ohmSymbol);
  encoder.begin();

  Serial.begin(9600);

  pinInit();

  controls.loadOff(lcd);

  welcomeScreen(lcd);
  delay(3000);
  mainMenu(lcd, userInput, keypad, encoder, measurements, controls);
}

void loop(){
  
}