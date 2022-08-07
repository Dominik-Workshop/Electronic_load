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

void mainMenu(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder){
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
		switch (keypad.getKey()){
				case '1':
					constCurrentMode(lcd, userInput, keypad, encoder);
					break;
				case '2':
					constPowerMode(lcd, userInput, keypad, encoder);
					break;
				case '3':
					constResistanceMode(lcd, userInput, keypad, encoder);
					break;
				case '4':
					transientResponseMode(lcd, userInput, keypad, encoder);
					break;
				case '5':
					batteryCapacityMode(lcd, userInput, keypad, encoder);
					break;
				default:
					delay(10);	//wait 10ms before checking again what keypad was pressed
					break;
		}
	}
}

void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder) {
	int x_pos = 0;
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Current");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set I=");
	lcd.setCursor(6,2);
  lcd.print(userInput.setCurrent, 3);
  lcd.setCursor(11,2);
  lcd.print("A");
		while(1){
			userInput.key = keypad.getKey();
			if(userInput.key == Menu) 
				mainMenu(lcd, userInput, keypad, encoder);	//return to menu
			else if(userInput.key == Enter){
				encoder.reset();
				userInput.time = millis();
				userInput.index = 0;
				x_pos = 0;
				while(userInput.time + 5000 > millis()){  //exit 5s of inactivity
					userInput.key = keypad.getKey();
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.') 
						inputFromKeypad(lcd, userInput, keypad, encoder, x_pos, userInput.setCurrent);
          if(encoder.wasButtonPressed()){
            if(userInput.decimalPlace > 0.001 )
              userInput.decimalPlace = userInput.decimalPlace/10;
            else 
              userInput.decimalPlace = 1;
          }
					if(encoder.rotation()){
						userInput.setCurrent += encoder.rotation() * userInput.decimalPlace;
						lcd.setCursor(6,2);
						lcd.print(userInput.setCurrent, 3);
						encoder.reset();
						userInput.time = millis();
					}
				}
			}
			delay(10);	//wait 10ms before checking again what keypad was pressed
		}
}

void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder){
	int x_pos = 0;
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Power");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set P=");
	lcd.setCursor(6,2);
  lcd.print(userInput.setPower, 3);
  lcd.setCursor(11,2);
  lcd.print("W");
	while(1){
			userInput.key = keypad.getKey();
			if(userInput.key == Menu) 
				mainMenu(lcd, userInput, keypad, encoder);	//return to menu
			else if(userInput.key == Enter){
				encoder.reset();
				userInput.time = millis();
				userInput.index = 0;
				x_pos = 0;
				while(userInput.time + 5000 > millis()){  //exit 5s of inactivity
					userInput.key = keypad.getKey();
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.') 
						inputFromKeypad(lcd, userInput, keypad, encoder, x_pos, userInput.setPower);
          if(encoder.wasButtonPressed()){
            if(userInput.decimalPlace > 0.001 )
              userInput.decimalPlace = userInput.decimalPlace/10;
            else 
              userInput.decimalPlace = 1;
          }
					if(encoder.rotation()){
						userInput.setPower += encoder.rotation() * userInput.decimalPlace;
						lcd.setCursor(6,2);
						lcd.print(userInput.setPower, 3);
						encoder.reset();
						userInput.time = millis();
					}
				}
			}
			delay(10);	//wait 10ms before checking again what keypad was pressed
		}
}

void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder){
	int x_pos = 0;
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Resistance");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set R=");
	lcd.setCursor(6,2);
  lcd.print(userInput.setResistance, 3);
  lcd.setCursor(11,2);
  lcd.write(ohm);
		while(1){
			userInput.key = keypad.getKey();
			if(userInput.key == Menu) 
				mainMenu(lcd, userInput, keypad, encoder);	//return to menu
			else if(userInput.key == Enter){
				encoder.reset();
				userInput.time = millis();
				userInput.index = 0;
				x_pos = 0;
				while(userInput.time + 5000 > millis()){  //exit 5s of inactivity
					userInput.key = keypad.getKey();
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.') 
						inputFromKeypad(lcd, userInput, keypad, encoder, x_pos, userInput.setResistance);
          if(encoder.wasButtonPressed()){
            if(userInput.decimalPlace > 0.001 )
              userInput.decimalPlace = userInput.decimalPlace/10;
            else 
              userInput.decimalPlace = 1;
          }
					if(encoder.rotation()){
						userInput.setResistance += encoder.rotation() * userInput.decimalPlace;
						lcd.setCursor(6,2);
						lcd.print(userInput.setResistance, 3);
						encoder.reset();
						userInput.time = millis();
					}
				}
			}
			delay(10);	//wait 10ms before checking again what keypad was pressed
		}
}

void transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder){

}

void batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder){

}

int inputFromKeypad(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, int& x_pos, float& setValue){
	char numbers[] = {'\0', '0','0','0','0','0','0'};	 //array of characters that will be converted to float later
	int index = 0;							                       //index for array numbers[]
	while(userInput.time + 5000 > millis()){
		if(userInput.key >= '0' && userInput.key <= '9'){               //check for keypad number input
			numbers[index++] = userInput.key;
			numbers[index] = '\0';
			lcd.setCursor(x_pos,3);                              
			lcd.print(userInput.key);                              //show number input on LCD
			x_pos++;
			userInput.time = millis();
		}

		if(userInput.key == '.'){                                   //Decimal point
			numbers[index++] = '.';
			numbers[index] = '\0';
			lcd.setCursor(x_pos,3);
			lcd.print(".");
			x_pos++;
		}

		if(userInput.key == Enter) {
			setValue = atof(numbers);
			lcd.setCursor(6,2);
			lcd.print(setValue, 3);
			lcd.setCursor(0,3);
			lcd.print("       ");
			x_pos = 0;
      encoder.reset();  //reset encoder in case it was turned while entering value via keypad
      return 0;
		}
    userInput.key = keypad.getKey();
	}
  encoder.reset();  //reset encoder in case it was turned while entering value via keypad
  lcd.setCursor(0,3);
	lcd.print("       ");
	return 0;
}