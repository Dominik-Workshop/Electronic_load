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

void mainMenu(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac){
	lcd.noCursor();
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
					constCurrentMode(lcd, userInput, keypad, encoder, adc, dac);
					break;
				case '2':
					constPowerMode(lcd, userInput, keypad, encoder, adc, dac);
					break;
				case '3':
					constResistanceMode(lcd, userInput, keypad, encoder, adc, dac);
					break;
				case '4':
					transientResponseMode(lcd, userInput, keypad, encoder, adc, dac);
					break;
				case '5':
					batteryCapacityMode(lcd, userInput, keypad, encoder, adc, dac);
					break;
				default:
					delay(10);	//wait 10ms before checking again what keypad was pressed
					break;
		}
	}
}

void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac) {
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
			displayTemperature(lcd);
			userInput.key = keypad.getKey();
			if(userInput.key == Menu) 
				mainMenu(lcd, userInput, keypad, encoder, adc, dac);	//return to menu
			else if(userInput.key == Enter){
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					displayTemperature(lcd);
					userInput.key = keypad.getKey();
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder, adc, dac);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.') 
						//inputFromKeypad(lcd, userInput, keypad, encoder, userInput.setCurrent);
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
				lcd.noCursor();
			}
			delay(10);	//wait 10ms before checking again what keypad was pressed
		}
}

void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac){
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Power");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set P=");
	lcd.setCursor(6,2);
  userInput.setPower.display(lcd);
  lcd.setCursor(12,2);
  lcd.print("W");
	userInput.cursorPos = 8;
	userInput.decimalPlace = 1;
	while(1){
			displayTemperature(lcd);
			userInput.key = keypad.getKey();
			if(userInput.key == Menu) 
				mainMenu(lcd, userInput, keypad, encoder, adc, dac);	//return to menu
			else if(userInput.key == Enter){
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					displayTemperature(lcd);
					userInput.key = keypad.getKey();
					lcd.setCursor(userInput.cursorPos,2);
					lcd.cursor();
					delay(100);
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder, adc, dac);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
						inputFromKeypad(lcd, userInput, keypad, userInput.setPower);
						encoder.reset();  //reset encoder in case it was turned while entering value via keypad
					}
          if(encoder.wasButtonPressed()){
						userInput.time = millis();
            if(userInput.decimalPlace > 0.01 ){
							userInput.decimalPlace = userInput.decimalPlace/10;
							++userInput.cursorPos;
						}
            else{
							userInput.decimalPlace = 100;
							userInput.cursorPos = 6;
						}
						if(userInput.decimalPlace == 0.1){
							++userInput.cursorPos;
						}
          }
					if(encoder.rotation()){
						userInput.setPower.value += encoder.rotation() * userInput.decimalPlace;
						userInput.setPower.limit();
						lcd.setCursor(6,2);
						userInput.setPower.display(lcd);
						lcd.setCursor(userInput.cursorPos,2);
						lcd.cursor();
						encoder.reset();
						userInput.time = millis();
					}
				}
				lcd.noCursor();
			}
			delay(10);	//wait 10ms before checking again what keypad was pressed
		}
}

void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac){
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Resistance");  
  lcd.setCursor(0,1);
  lcd.print("00.000V 0.000A 0.00W");
  lcd.setCursor(0,2);
  lcd.print("Set R=");
	lcd.setCursor(6,2);
  userInput.setResistance.display(lcd);
  lcd.setCursor(12,2);
  lcd.write(ohm);
	userInput.cursorPos = 9;
	userInput.decimalPlace = 1;
		while(1){
			displayTemperature(lcd);
			dac.setVoltage(userInput.setResistance.value, false);
			lcd.setCursor(0,3);
			lcd.print(adc.computeVolts(adc.readADC_SingleEnded(0)));
			userInput.key = keypad.getKey();
			if(userInput.key == Menu) 
				mainMenu(lcd, userInput, keypad, encoder, adc, dac);	//return to menu
			else if(userInput.key == Enter){
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					displayTemperature(lcd);
					userInput.key = keypad.getKey();
					lcd.setCursor(userInput.cursorPos,2);
					lcd.cursor();
					delay(100);
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder, adc, dac);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
						inputFromKeypad(lcd, userInput, keypad, userInput.setResistance);
						encoder.reset();  //reset encoder in case it was turned while entering value via keypad
					}
          if(encoder.wasButtonPressed()){
						userInput.time = millis();
            if(userInput.decimalPlace > 0.1 ){
							userInput.decimalPlace = userInput.decimalPlace/10;
							++userInput.cursorPos;
						}
            else{
							userInput.decimalPlace = 1000;
							userInput.cursorPos = 6;
						}
						if(userInput.decimalPlace == 0.1){	//jump across the decimal point
							++userInput.cursorPos;
						}
          }
					if(encoder.rotation()){
						userInput.setResistance.value += encoder.rotation() * userInput.decimalPlace;
						userInput.setResistance.limit();
						lcd.setCursor(6,2);
						userInput.setResistance.display(lcd);
						lcd.setCursor(userInput.cursorPos,2);
						lcd.cursor();
						encoder.reset();
						userInput.time = millis();
					}
				}
				lcd.noCursor();
			}
			delay(10);	//wait 10ms before checking again what keypad was pressed
		}
}

void transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac){
	dac.setVoltage(0, false);
	delay(1000);
	for(int i = 0; i< 4096; ++i){
		dac.setVoltage(i, false);
		delay(1);
	}
}

void batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Adafruit_ADS1115& adc, Adafruit_MCP4725& dac){

}

int inputFromKeypad(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, SetValue& setParameter){
	char numbers[7] = {'\0', '0','0','0','0','0'};	 //array of characters that will be converted to float later
	int index = 0;							                       //index for array numbers[]
	bool decimalPointPresent = false;									 //indicates if user already input a decimal point
	int x_pos = 0;																	 	 //cursor position for entered numbers
	lcd.noCursor();

	while(userInput.time + 5000 > millis()){
		if(index <=5){	//limit number of input characters  
			if(userInput.key >= '0' && userInput.key <= '9'){	//is digit
				numbers[index] = userInput.key;
				numbers[++index] = '\0';
				lcd.setCursor(x_pos,3);                              
				lcd.print(userInput.key);  //show number input on LCD
				x_pos++;
				userInput.time = millis();
			}

			if(userInput.key == '.' && !decimalPointPresent){
				numbers[index] = '.';
				numbers[++index] = '\0';
				lcd.setCursor(x_pos,3);
				lcd.print(".");
				x_pos++;
				decimalPointPresent = true;
			}
		}
		
		if(userInput.key == Delete){
			lcd.setCursor(--x_pos,3);
			lcd.print(" ");
			numbers[--index] = '\0';
			if(index == 0) 
				return 1;	//if deleted all numbers exit function
		}

		if(userInput.key == Enter) {
			setParameter.value = atof(numbers);		//convert array of entered numbers to float and write it to SetValue variable
			setParameter.limit();
			lcd.setCursor(6,2);
			setParameter.display(lcd);
			lcd.setCursor(0,3);
			lcd.print("       ");		//clear all entered numbers from the screen
			decimalPointPresent = false;
      return 0;
		}
    userInput.key = keypad.getKey();
	}
  lcd.setCursor(0,3);
	lcd.print("       ");		//clear all entered numbers from the screen
	return 0;
}

void displayTemperature(LiquidCrystal_I2C& lcd){
	lcd.setCursor(16,3);
	lcd.print(measureTemperature());
	lcd.write(degree);
	lcd.print("C");
}