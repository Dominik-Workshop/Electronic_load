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
#include "lcd_characters.hh"

void constCurrentMode(LiquidCrystal_I2C& lcd) {
  lcd.setCursor(0,0);
  lcd.print("DC LOAD");  
  lcd.setCursor(0,2);
  lcd.print("                ");
  lcd.setCursor(0,2);
  lcd.print("Set I = ");
  lcd.setCursor(16,2);
  lcd.print("    ");
  lcd.setCursor(14,2);
  lcd.print("A");
  lcd.setCursor(0,3);
}

void setup() {
  CalibrationValues calibrationValues;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Adafruit_ADS1115 adc;
  Adafruit_MCP4725 dac;
  Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
  Encoder encoder;
  
  char numbers[10];
  int index, z;
  char customKey;
  float value = 0;
  uint32_t time;

  lcd.init();
  lcd.backlight();
  lcd.createChar(degree, degreeSymbol);
  lcd.createChar(ohm, ohmSymbol);
  adc.begin();
  adc.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  dac.begin(DAC_ADDRESS);
  encoder.begin();

  Serial.begin(9600);

  pinInit();
  calibrationValues.readFromEEPROM();

  lcd.setCursor(1,0);
  lcd.print("DC Electronic Load");
  lcd.setCursor(2,1);
  lcd.print("Dominik Workshop");
  lcd.setCursor(8,2);
  lcd.print("2022");
  lcd.setCursor(4,3);
  lcd.print("5A 30V 150W");
  
  delay(3000);

  lcd.clear();

  constCurrentMode(lcd);

  while(1){
    
    digitalWrite(13, HIGH);
    delay(20);
    digitalWrite(13, LOW);
    delay(20); 
    lcd.setCursor(16,0);
    lcd.print(measureTemperature());
    lcd.write(degree);
    lcd.print("C");

    customKey = keypad.getKey();
    if(customKey == '#'){
      encoder.reset();
      time = millis();
      index = 0;
      z = 0;
      while(time + 5000 > millis()){  //exit 5s after last encoder movement
        customKey = keypad.getKey();
        if(customKey >= '0' && customKey <= '9'){               //check for keypad number input
          numbers[index++] = customKey;
          numbers[index] = '\0';
          lcd.setCursor(z,3);                              
          lcd.print(customKey);                              //show number input on LCD
          z++;
          time = millis();
        }

        if(customKey == '*'){                                   //Decimal point
            numbers[index++] = '.';
            numbers[index] = '\0';
            lcd.setCursor(z,3);
            lcd.print(".");
            z = z+1;
        }

        if(encoder.rotation()){
          value += encoder.rotation();
          lcd.setCursor(8,2);
          lcd.print(value, 3);
          lcd.print(" ");
          encoder.reset();
          time = millis();
        } 
        if(customKey == '#') {
          value = atof(numbers);
          lcd.setCursor(8,2);
          lcd.print(value, 3);
          lcd.print(" ");
          // if(value < 10) lcd.print(" ");
          // if(value < 100) lcd.print(" ");
          // if(value < 1000) lcd.print(" ");
          lcd.setCursor(0,3);
          lcd.print("       ");
          index = 0;
          z = 0;
        }
      }
    }
    
  //   if(encoder.wasButtonPressed()) {
  //     lcd.setCursor(3,1);
  //     lcd.print("Button pressed");
  //   }
  //   else{
  //     lcd.setCursor(3,1);
  //     lcd.print("               ");
  //   }
    
  //   lcd.setCursor(3,2);
  //   lcd.print(encoder.getCounts());
  //   lcd.print("   ");

  //   lcd.setCursor(3,3);
  //   lcd.print("T=");
  //   lcd.print(measureTemperature());
  //   lcd.print("   ");

  //   customKey = keypad.getKey();
     
  //   if (customKey) {
  //     lcd.setCursor(9,4);
  //     lcd.print(customKey);
  //   }
  }
}

void loop(){
  
}