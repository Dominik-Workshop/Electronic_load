/**
 * @file mode_screen.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 2.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "mode_screen.hh"

void displayWelcomeScreen(LiquidCrystal_I2C& lcd){
	lcd.clear();
	lcd.setCursor(1,0);
  lcd.print("DC Electronic Load");
  lcd.setCursor(2,1);
  lcd.print("Dominik Workshop");
  lcd.setCursor(8,2);
  lcd.print("2022");
  lcd.setCursor(4,3);
  lcd.print("8,4A 50V 300W");
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
	taskLoop(ConstCurrent, userInput.setCurrent, lcd, userInput, keypad, encoder, measurements, controls);
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
	taskLoop(ConstPower, userInput.setPower, lcd, userInput, keypad, encoder, measurements, controls);
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
	taskLoop(ConstResistance, userInput.setResistance, lcd, userInput, keypad, encoder, measurements, controls);
}

void transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){

}

void batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	Battery battery;
	lcd.clear();
	lcd.setCursor(0 ,0);
	lcd.print("  Battery capacity  ");
  lcd.setCursor(0, 1);
	lcd.print("  measurement mode  ");
	lcd.setCursor(0, 2);
  lcd.print("enter cutoff voltage");
	lcd.setCursor(0, 3);


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

/**
 * @brief updates and displays measurements, controls the fan, takes input from the keypad and encoder, controls the load depending on mode of operation
 * 
 * @param mode indicates if it's constant current, power, or resistance mode
 * @param setParameter setCurrent, setPower or setResistance
 * @param lcd 
 * @param userInput 
 * @param keypad 
 * @param encoder 
 * @param measurements 
 * @param controls 
 */
void taskLoop(ModeOfOperation mode, SetValue& setParameter, LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		controls.fanControll();
		loadControl(controls, userInput, mode);
		switch (keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				userInput.resetKeypadInput();
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
					measurements.displayMeasurements(lcd, controls.isLoadOn());
					controls.fanControll();
					loadControl(controls, userInput, mode);

					userInput.key = keypad.getKey();
					lcd.cursor();
					lcd.setCursor(userInput.cursorPos,2);
					delay(100);
					if(userInput.key == Menu){
						controls.loadOff(lcd);
						userInput.resetKeypadInput();
						mainMenu(lcd, userInput, keypad, encoder, measurements, controls);
					}
					else if(userInput.key == LoadOnOff)
						controls.loadOnOffToggle(lcd);

					userInput.inputFromKeypad(lcd, keypad, setParameter);
					
					checkEncoder(lcd, userInput, setParameter, encoder);
				}
				lcd.noCursor();
				break;
			default:
				delay(10);	//wait 10ms before checking again what keypad was pressed
				break;
		}
	}
}

/**
 * @brief depending on mode of operation controls power transistors in such a way to obtain
 * set constant current, power or resistance
 * 
 * @param controls 
 * @param userInput contains information about set values
 * @param mode indicates if it's constant current, power, or resistance mode
 */
void loadControl(Controls& controls, UserInput& userInput, ModeOfOperation mode){
	switch (mode){
	case ConstCurrent:
		controls.regulateCurrent(userInput.setCurrent.value);
		break;
	case ConstPower:
		controls.regulatePower(userInput.setPower.value);
		break;
	case ConstResistance:
		controls.regulateResistance(userInput.setResistance.value);
		break;
	default:
		break;
	}
}

void checkEncoder(LiquidCrystal_I2C& lcd, UserInput& userInput, SetValue& setParameter, Encoder& encoder){
	if(encoder.wasButtonPressed()){
		userInput.time = millis();
		if(userInput.decimalPlace > setParameter.minDecimalPlace){	//if did't reach the last digit of setParameter
			--userInput.decimalPlace;																	//move cursor to the left
			++userInput.cursorPos;
		}   
		else {
			userInput.decimalPlace = setParameter.maxDecimalPlace;		//move the cursor back to the first digit of setParameter
			userInput.cursorPos = 6;
		}
		if(userInput.decimalPlace == tenths)	//jump across the decimal point
			++userInput.cursorPos;
	}

	if(encoder.rotation()){
		setParameter.value += encoder.rotation() * pow(10, userInput.decimalPlace);
		setParameter.limit();
		lcd.setCursor(6,2);
		setParameter.display(lcd);
		encoder.reset();
		userInput.time = millis();
	}
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

/**
 * @brief calibrate ADC and DAC (measured voltage and current, set current)
 * 
 * @param lcd 
 * @param userInput 
 * @param keypad 
 * @param encoder 
 * @param measurements 
 * @param controls 
 */
void calibration(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	int lastCalibrationValue;

	controls.loadOff(lcd);	//don't sink any current
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Calibration mode");
	delay(1500);
	//////////////////////////////////////////////
	// measured voltage multiplier calibration	//
	//////////////////////////////////////////////
	lcd.setCursor(0, 0);
	lcd.print("Calibrate voltage   ");
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");
	lastCalibrationValue = measurements.calibration.getVoltageMultiplier();
	encoder.reset();
	while(keypad.getKey() != Enter){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setVoltageMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15,2);
		lcd.print(measurements.calibration.getVoltageMultiplier());
		lcd.print("  ");
		delay(100);
	}

	//////////////////////////////////////////////
	// measured current multiplier calibration	//
	//////////////////////////////////////////////
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("   !!!WARNING!!!    ");
	lcd.setCursor(0, 1);
	lcd.print(" Load's input will  ");
	lcd.setCursor(0, 2);
	lcd.print("  be shorted now!   ");
	delay(3000);

	controls.loadOn(lcd);
	controls.dac.setVoltage(4095, false);		//short the input of the load

	lcd.setCursor(0, 0);
	lcd.print("Calibrate current   ");
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");

	lastCalibrationValue = measurements.calibration.getCurrentMultiplier();
	encoder.reset();
	while(keypad.getKey() != Enter){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setCurrentMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15, 2);
		lcd.print(measurements.calibration.getCurrentMultiplier());
		lcd.print("  ");
		delay(100);
	}

	//////////////////////////////////////////////
	// 		measured current offset calibration		//
	//////////////////////////////////////////////
	lcd.setCursor(0, 2);
	lcd.print("cal offset=");
	lastCalibrationValue = measurements.calibration.getCurrentOffset();
	encoder.reset();
	while(keypad.getKey() != Enter){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setCurrentOffset(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(11, 2);
		lcd.print(measurements.calibration.getCurrentOffset());
		lcd.print("      ");
		delay(100);
	}

	//////////////////////////////////////////////
	// 		set current multiplier calibration		//
	//////////////////////////////////////////////
	lcd.setCursor(0, 0);
	lcd.print("Calibrate setCurrent");
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");
	lastCalibrationValue = controls.calibration.getSetCurrentMultiplier();
	encoder.reset();
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		userInput.key = keypad.getKey();
		if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
			userInput.time = millis();
			userInput.inputFromKeypad(lcd, keypad, userInput.setCurrent);
			lcd.setCursor(0, 2);
			lcd.print("cal multiplier=");
			userInput.key = ' ';
		}
		else if(userInput.key == Enter)
			break;

		controls.regulateCurrent(userInput.setCurrent.value);
		controls.calibration.setSetCurrentMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15, 2);
		lcd.print(controls.calibration.getSetCurrentMultiplier());
		lcd.print("  ");
		delay(100);
	}

	//////////////////////////////////////////////
	// 			set current offset calibration			//
	//////////////////////////////////////////////
	lcd.setCursor(0, 2);
	lcd.print("cal offset=");
	lastCalibrationValue = controls.calibration.getSetCurrentOffset();
	encoder.reset();
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		userInput.key = keypad.getKey();
		if((userInput.key >= '0' && userInput.key <= '9') || userInput.key == '.'){
			userInput.time = millis();
			userInput.inputFromKeypad(lcd, keypad, userInput.setCurrent);
			userInput.key = ' ';
			lcd.setCursor(0, 2);
			lcd.print("cal offset=");
		}
		else if(userInput.key == Enter)
			break;

		controls.regulateCurrent(userInput.setCurrent.value);
		controls.calibration.setSetCurrentOffset(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(11, 2);
		lcd.print(controls.calibration.getSetCurrentOffset());
		lcd.print("      ");
		delay(100);
	}

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Save new calibration");
	lcd.setCursor(0, 1);
	lcd.print(" values to EEPROM?  ");
	lcd.setCursor(0, 2);
	lcd.print("   Yes = Enter");
	lcd.setCursor(0, 3);
	lcd.print("   No  = Delete");

	userInput.key = keypad.getKey();
	while(userInput.key != Delete){
		if(userInput.key == Enter){
			measurements.calibration.writeToEEPROM();
			controls.calibration.writeToEEPROM();
			break;
		}
		delay(100);
		userInput.key = keypad.getKey();
	}

	controls.loadOff(lcd);
	mainMenu(lcd, userInput, keypad, encoder, measurements, controls);	//return to menu
}