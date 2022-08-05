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
#include "user_input.hh"

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
  UserInput userInput;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Adafruit_ADS1115 adc;
  Adafruit_MCP4725 dac;
  Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
  Encoder encoder;
  
  int x_pos;    //horizontal position of lcd cursor
  

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

    userInput.key = keypad.getKey();
    if(userInput.key == '#'){
      encoder.reset();
      userInput.time = millis();
      userInput.index = 0;
      x_pos = 0;
      while(userInput.time + 5000 > millis()){  //exit 5s of inactivity
        userInput.key = keypad.getKey();
        if(userInput.key >= '0' && userInput.key <= '9'){               //check for keypad number input
          userInput.numbers[userInput.index++] = userInput.key;
          userInput.numbers[userInput.index] = '\0';
          lcd.setCursor(x_pos,3);                              
          lcd.print(userInput.key);                              //show number input on LCD
          x_pos++;
          userInput.time = millis();
        }

        if(userInput.key == '*'){                                   //Decimal point
            userInput.numbers[userInput.index++] = '.';
            userInput.numbers[userInput.index] = '\0';
            lcd.setCursor(x_pos,3);
            lcd.print(".");
            x_pos++;
        }

        if(encoder.rotation()){
          userInput.value += encoder.rotation();
          lcd.setCursor(8,2);
          lcd.print(userInput.value, 3);
          lcd.print(" ");
          encoder.reset();
          userInput.time = millis();
        }

        if(userInput.key == '#') {
          userInput.value = atof(userInput.numbers);
          lcd.setCursor(8,2);
          lcd.print(userInput.value, 3);
          lcd.print(" ");
          lcd.setCursor(0,3);
          lcd.print("       ");
          userInput.index = 0;
          x_pos = 0;
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

  //   userInput.key = keypad.getKey();
     
  //   if (userInput.key) {
  //     lcd.setCursor(9,4);
  //     lcd.print(userInput.key);
  //   }
  }
}

void loop(){
  
}