/**
 * @file main.cpp
 * @author Dominik Workshop
 * @brief Electronic load with 4 different modes of operation:
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

void setup() {
  CalibrationValues calibrationValues;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Adafruit_ADS1115 adc;
  Adafruit_MCP4725 dac;
  Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
  Encoder encoder;
  
  char customKey;
  //int counts;

  lcd.init();
  lcd.backlight();
  adc.begin();
  adc.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  dac.begin(DAC_ADDRESS);
  encoder.begin();

  Serial.begin(9600);

  pinInit();
  calibrationValues.readFromEEPROM();

  lcd.setCursor(3,0);
  lcd.print("Hello, world!");

  while(1){
    
    digitalWrite(13, HIGH);
    delay(20);
    digitalWrite(13, LOW);
    delay(20); 
    
    if(encoder.wasButtonPressed()) {
      lcd.setCursor(3,1);
      lcd.print("Button pressed");
    }
    else{
      lcd.setCursor(3,1);
      lcd.print("               ");
    }
    
    lcd.setCursor(3,2);
    lcd.print(encoder.getCounts());
    lcd.print("   ");

    lcd.setCursor(3,3);
    lcd.print("T=");
    lcd.print(measureTemperature());
    lcd.print("   ");

    customKey = keypad.getKey();
     
    if (customKey) {
      lcd.setCursor(9,4);
      lcd.print(customKey);
    }
  }
}

void loop(){
  
}