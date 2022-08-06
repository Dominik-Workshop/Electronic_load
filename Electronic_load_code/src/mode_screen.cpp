/**
 * @file mode_screen.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "mode_screen.hh"
#include "lcd_characters.hh"

void welcomeScreen(LiquidCrystal_I2C& lcd){
	lcd.clear();
	lcd.setCursor(1,0);
  lcd.print("DC Electronic Load");
  lcd.setCursor(2,1);
  lcd.print("Dominik Workshop");
  lcd.setCursor(8,2);
  lcd.print("2022");
  lcd.setCursor(4,3);
  lcd.print("5A 30V 150W");
}

void mainMenu(LiquidCrystal_I2C& lcd, UserInput& userInput){
	lcd.clear();
	lcd.setCursor(0,0);
  lcd.print("1.Const. Current");  
  lcd.setCursor(0,1);
  lcd.print("2.Const. Power");
  lcd.setCursor(0,2);
  lcd.print("3.Const. Resistance");
  lcd.setCursor(0,3);
  lcd.print("4.Transient  5.Batt.");

	while(1){
		switch (userInput.keypad.getKey()){
				case '1':
					constCurrentMode(lcd, userInput);
					break;
				case '2':
					constPowerMode(lcd, userInput);
					break;
				case '3':
					constResistanceMode(lcd, userInput);
					break;
				default:
					delay(10);	//wait 10ms before checking again what keypad was pressed
					break;
		}
	}
}

void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput) {
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Current");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set I=");
  lcd.setCursor(11,2);
  lcd.print("A");
	while(1){
		if(userInput.keypad.getKey() == 'A') mainMenu(lcd, userInput);
		delay(10);	//wait 10ms before checking again what keypad was pressed
	}
}

void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput){
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Power");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set P=");
	lcd.setCursor(6,2);
  lcd.print(userInput.value, 3);
  lcd.setCursor(11,2);
  lcd.print("W");
	while(1){
		if(userInput.keypad.getKey() == 'A') 
			mainMenu(lcd, userInput);
		//else if(userInput.lastKey == 'B')
		if(userInput.encoder.rotation()){
      userInput.value += userInput.encoder.rotation();
      lcd.setCursor(6,2);
      lcd.print(userInput.value, 3);
      userInput.encoder.reset();
      userInput.time = millis();
    }
		delay(10);	//wait 10ms before checking again what keypad was pressed
	}
}

void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput){
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Resistance");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set R=");
  lcd.setCursor(11,2);
  lcd.write(ohm);
	while(1){
		if(userInput.keypad.getKey() == 'A') mainMenu(lcd, userInput);
		delay(10);	//wait 10ms before checking again what keypad was pressed
	}
}