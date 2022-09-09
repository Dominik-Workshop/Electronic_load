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
  lcd.print("8,4A 60V 300W");
}

void mainMenu(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	displayMenu(lcd);
	while(1){
		switch (keypad.getKey()){
			case '1':
				constCurrentMode(lcd, userInput, keypad, encoder, measurements, controls);
				break;
			case '2':
				constPowerMode(lcd, userInput, keypad, encoder, measurements, controls);
				break;
			case '3':
				constResistanceMode(lcd, userInput, keypad, encoder, measurements, controls);
				break;
			case '4':
				transientResponseMode(lcd, userInput, keypad, encoder, measurements, controls);
				break;
			case '5':
				batteryCapacityMode(lcd, userInput, keypad, encoder, measurements, controls);
				break;
			case '6':
				calibration(lcd, userInput, keypad, encoder, measurements, controls);
				break;
			default:
				delay(10);	//wait 10ms before checking again what keypad was pressed
				break;
		}
	}
}

void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls) {
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Current    OFF");  
  lcd.setCursor(0,2);
  lcd.print("Set I=");
  userInput.setCurrent.display(lcd);
  lcd.print("A");
	userInput.cursorPos = 6;
	userInput.decimalPlace = ones;
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd);
		controls.fanControll(measurements);
		controls.sinkCurrent(userInput.setCurrent.value);
		switch (keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				mainMenu(lcd, userInput, keypad, encoder, measurements, controls);	//return to menu				
				break;
			case LoadOnOff:
				controls.loadOnOffToggle(lcd);
				break;
			case Enter:
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					measurements.update();
					measurements.displayMeasurements(lcd);
					controls.fanControll(measurements);
					controls.sinkCurrent(userInput.setCurrent.value);
					userInput.key = keypad.getKey();
					lcd.setCursor(userInput.cursorPos,2);
					lcd.cursor();
					delay(100);
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder, measurements, controls);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
						inputFromKeypad(lcd, userInput, keypad, userInput.setCurrent);
						encoder.reset();  //reset encoder in case it was turned while entering value via keypad
					} 
					checkEncoder(lcd, userInput, userInput.setCurrent, encoder);
				}
				lcd.noCursor();
				break;
			default:
				delay(10);	//wait 10ms before checking again what keypad was pressed
				break;
		}
	}
}

void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Power      OFF");  
  lcd.setCursor(0,2);
  lcd.print("Set P=");
  userInput.setPower.display(lcd);
  lcd.print("W");
	userInput.cursorPos = 8;
	userInput.decimalPlace = ones;
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd);
		controls.fanControll(measurements);
		controls.drawConstPower(userInput.setPower.value);
		switch (keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				mainMenu(lcd, userInput, keypad, encoder, measurements, controls);	//return to menu				
				break;
			case LoadOnOff:
				controls.loadOnOffToggle(lcd);
				break;
			case Enter:
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					measurements.update();
					measurements.displayMeasurements(lcd);
					controls.fanControll(measurements);
					controls.drawConstPower(userInput.setPower.value);
					userInput.key = keypad.getKey();
					lcd.setCursor(userInput.cursorPos,2);
					lcd.cursor();
					delay(100);
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder, measurements, controls);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
						inputFromKeypad(lcd, userInput, keypad, userInput.setPower);
						encoder.reset();  //reset encoder in case it was turned while entering value via keypad
					} 
					checkEncoder(lcd, userInput, userInput.setPower, encoder);
				}
				lcd.noCursor();
				break;
			default:
				delay(10);	//wait 10ms before checking again what keypad was pressed
				break;
		}
	}
}

void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Const Resistance OFF");  
  lcd.setCursor(0,2);
  lcd.print("Set R=");
  userInput.setResistance.display(lcd);
  lcd.write(ohm);
	userInput.cursorPos = 9;
	userInput.decimalPlace = ones;
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd);
		controls.fanControll(measurements);
		controls.constResistance(userInput.setResistance.value);
		switch (keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				mainMenu(lcd, userInput, keypad, encoder, measurements, controls);	//return to menu
				break;
			case LoadOnOff:
				controls.loadOnOffToggle(lcd);
				break;
			case Enter:
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					measurements.update();
					measurements.displayMeasurements(lcd);
					controls.fanControll(measurements);
					controls.constResistance(userInput.setResistance.value);
					userInput.key = keypad.getKey();
					lcd.setCursor(userInput.cursorPos,2);
					lcd.cursor();
					delay(100);
					if(userInput.key == Menu) 
						mainMenu(lcd, userInput, keypad, encoder, measurements, controls);
					else if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
						inputFromKeypad(lcd, userInput, keypad, userInput.setResistance);
						encoder.reset();  //reset encoder in case it was turned while entering value via keypad
					} 
					checkEncoder(lcd, userInput, userInput.setResistance, encoder);
				}
				lcd.noCursor();
				break;
			default:
				delay(10);	//wait 10ms before checking again what keypad was pressed
				break;
		}
	}
}

void transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	controls.dac.setVoltage(0, false);
	delay(1000);
	for(int i = 0; i< 4096; ++i){
		controls.dac.setVoltage(i, false);
		delay(1);
	}
}

void batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	lcd.clear();
	measurements.timer.start();
	while(keypad.getKey() != Menu){
		lcd.setCursor(0, 0);
		lcd.print(measurements.timer.getTotalSeconds());
		lcd.setCursor(0, 1);
		lcd.print(measurements.timer.getTime());
		delay(100);
	}
	measurements.timer.stop();
	measurements.timer.reset();
	displayMenu(lcd);
}

void calibration(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	int lastCalibrationValue;

	controls.loadOff(lcd);	//don't sink any current
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Calibration mode");
	delay(2000);
	lcd.setCursor(0, 0);
	lcd.print("Calibrate voltage ");
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");
	lastCalibrationValue = measurements.calibration.getVoltageMultiplier();
	encoder.reset();
	while(keypad.getKey() != Enter){
		measurements.update();
		measurements.displayMeasurements(lcd);
		measurements.calibration.setVoltageMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15,2);
		lcd.print(measurements.calibration.getVoltageMultiplier());
		lcd.print("  ");
	}
	lcd.setCursor(0, 0);
	lcd.print("Calibrate current ");
	controls.loadOn(lcd);
	controls.dac.setVoltage(4095, false);		//short the input of the load
	lastCalibrationValue = measurements.calibration.getCurrentMultiplier();
	encoder.reset();
	while(keypad.getKey() != Enter){
		measurements.update();
		measurements.displayMeasurements(lcd);
		measurements.calibration.setCurrentMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15,2);
		lcd.print(measurements.calibration.getCurrentMultiplier());
		lcd.print("  ");
	}

	lcd.setCursor(0, 2);
	lcd.print("cal offset    =");
	lastCalibrationValue = measurements.calibration.getCurrentOffset();
	encoder.reset();
	while(keypad.getKey() != Enter){
		measurements.update();
		measurements.displayMeasurements(lcd);
		measurements.calibration.setCurrentOffset(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15,2);
		lcd.print(measurements.calibration.getCurrentOffset());
		lcd.print("  ");
	}

	lcd.setCursor(0, 0);
	lcd.print("Calibrate setCurrent");
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");
	lastCalibrationValue = controls.calibration.getSetCurrentMultiplier();
	encoder.reset();
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd);
		userInput.key = keypad.getKey();
		if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
			userInput.time = millis();
			inputFromKeypad(lcd, userInput, keypad, userInput.setCurrent);
		}
		else if(userInput.key == '#')
			break;
		controls.sinkCurrent(userInput.setCurrent.value);
		controls.calibration.setSetCurrentMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15,2);
		lcd.print(controls.calibration.getSetCurrentMultiplier());
		lcd.print("  ");
		
	}
	measurements.calibration.writeToEEPROM();
	controls.calibration.writeToEEPROM();

	controls.loadOff(lcd);
	mainMenu(lcd, userInput, keypad, encoder, measurements, controls);	//return to menu
}

int inputFromKeypad(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, SetValue& setParameter){
	char numbers[7] = {'\0', '0','0','0','0','0'};	 	 //array of characters that will be converted to float later
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
			--index;
			if(numbers[index] == '.') //deleted decimal point
				decimalPointPresent = false;
			numbers[index] = '\0';
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

void displayMenu(LiquidCrystal_I2C& lcd){
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
}

void checkEncoder(LiquidCrystal_I2C& lcd, UserInput& userInput, SetValue& setValue, Encoder& encoder){
	if(encoder.wasButtonPressed()){
		userInput.time = millis();
		if(userInput.decimalPlace > setValue.minDecimalPlace){
			--userInput.decimalPlace;
			++userInput.cursorPos;
		}   
		else {
			userInput.decimalPlace = setValue.maxDecimalPlace;
			userInput.cursorPos = 6;
		}
		if(userInput.decimalPlace == tenths)	//jump across the decimal point
			++userInput.cursorPos;
	}
	if(encoder.rotation()){
		setValue.value += encoder.rotation() * pow(10, userInput.decimalPlace);
		setValue.limit();
		lcd.setCursor(6,2);
		setValue.display(lcd);
		lcd.setCursor(userInput.cursorPos,2);
		lcd.cursor();
		encoder.reset();
		userInput.time = millis();
	}
}