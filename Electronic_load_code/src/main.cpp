/**
 * @file main.cpp
 * @author Dominik Workshop
 * @brief Electronic load with 5 different modes of operation:
 *  - Constant current,
 *  - Constant power,
 *  - Constant resistance,
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

#include "defines.hh"
#include "init.hh"
#include "calibration.hh"
#include "temperature.hh"
#include "encoder.hh"
#include "keypad_config.hh"
#include "lcd_characters.hh"
#include "mode_screen.hh"
#include "user_input.hh"


void setup() {
  CalibrationValues calibrationValues;
  UserInput userInput;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Adafruit_ADS1115 adc;
  Adafruit_MCP4725 dac;
  Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
  Encoder encoder;
  
  //int x_pos;    //horizontal position of lcd cursor

  lcd.init();
  lcd.backlight();
  lcd.createChar(degree, degreeSymbol);
  lcd.createChar(ohm, ohmSymbol);
  adc.begin(ADC_ADDRESS);
  adc.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  dac.begin(DAC_ADDRESS);
  encoder.begin();

  Serial.begin(9600);

  pinInit();
  calibrationValues.readFromEEPROM();

  welcomeScreen(lcd);
  delay(3000);
  mainMenu(lcd, userInput, keypad, encoder);
}

void loop(){
  
}