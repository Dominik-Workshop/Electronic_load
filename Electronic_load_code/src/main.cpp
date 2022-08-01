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

void setup() {
  CalibrationValues calibrationValues;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Adafruit_ADS1115 adc;
  Adafruit_MCP4725 dac;

  lcd.init();  
  adc.begin();
  adc.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  dac.begin(DAC_ADDRESS);

  Serial.begin(9600);


  pinInit();
  calibrationValues.readFromEEPROM();

  // Print a message to the LCD.
  //lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
}

void loop(){
  //Serial.println(measureTemperature());
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100); 
  Serial.println(EncoderValue);
}