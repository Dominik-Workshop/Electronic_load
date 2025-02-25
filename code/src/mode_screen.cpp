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
	lcd.setCursor(1, 0);
  lcd.print("DC Electronic Load");
  lcd.setCursor(2, 1);
  lcd.print("Dominik Workshop");
  lcd.setCursor(8, 2);
  lcd.print("2022");
  lcd.setCursor(4, 3);
  lcd.print("8,4A 50V 300W");
}

void displayMenu(LiquidCrystal_I2C& lcd){
	lcd.noCursor();
	lcd.clear();
	lcd.setCursor(0, 0);
  lcd.print("1.Const. Current");  
  lcd.setCursor(0, 1);
  lcd.print("2.Const. Power");
  lcd.setCursor(0, 2);
  lcd.print("3.Const. Resistance");
  lcd.setCursor(0, 3);
  lcd.print("4.Transient  5.Batt.");
}

void constCurrentMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls) {
	lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Const Current    OFF");  
  lcd.setCursor(0, 2);
  lcd.print("Set I=");
  userInput.setCurrent.display(lcd);
  lcd.print("A");
	userInput.cursorPosX = 6;
	userInput.decimalPlace = ones;
	taskLoop(ConstCurrent, userInput.setCurrent, lcd, userInput, keypad, encoder, measurements, controls);
}

void constPowerMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Const Power      OFF");  
  lcd.setCursor(0, 2);
  lcd.print("Set P=");
  userInput.setPower.display(lcd);
  lcd.print("W");
	userInput.cursorPosX = 8;
	userInput.decimalPlace = ones;
	taskLoop(ConstPower, userInput.setPower, lcd, userInput, keypad, encoder, measurements, controls);
}

void constResistanceMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Const Resistance OFF");  
  lcd.setCursor(0, 2);
  lcd.print("Set R=");
  userInput.setResistance.display(lcd);
  lcd.write(ohm);
	userInput.cursorPosX = 9;
	userInput.decimalPlace = ones;
	taskLoop(ConstResistance, userInput.setResistance, lcd, userInput, keypad, encoder, measurements, controls);
}

int transientResponseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls, Transient& transient){
	lcd.setCursor(0, 0);
	lcd.print(" Transient response ");
  lcd.setCursor(0, 1);
	lcd.print("1.Continous  2.Pulse");
	lcd.setCursor(0, 2);
  lcd.print("3.Toggle            ");
	lcd.setCursor(0, 3);
  lcd.print("              4.Exit");

	do{
		userInput.key = keypad.getKey();
	} while (!(userInput.key >= '1' && userInput.key <= '4'));

	switch (userInput.key){
		case '1':
			transient.continousMode(lcd, userInput, keypad, encoder, measurements, controls);
			break;
		case '2':
			transient.pulseMode(lcd, userInput, keypad, encoder, measurements, controls);
			break;
		case '3':
			transient.toggleMode(lcd, userInput, keypad, encoder, measurements, controls);
			break;
		case '4':	//Exit
			break;
	}
	return 0;
}

uint32_t calculateCRC(const uint8_t* data, size_t length) {
  const uint32_t polynomial = 0xEDB88320;
  uint32_t crc = 0xFFFFFFFF;

  for (size_t i = 0; i < length; ++i) {
    crc ^= data[i];
    for (size_t j = 0; j < 8; ++j) {
      crc = (crc >> 1) ^ (-(int)(crc & 1) & polynomial);
    }
  }

  return crc ^ 0xFFFFFFFF;
}

/**
 * @brief Sends load statistics including voltage, current, temperature,
 * cutoff voltage, discharge current, load status, and total operating time
 * over the serial communication.
 *
 * @param measurements Reference to the Measurements object containing load measurements.
 * @param battery Reference to the Battery object containing battery-related information.
 * @param isLoadOnBool Boolean indicating whether the load is turned on.
 */
void sendLoadStatisticsOverSerial(Measurements& measurements, Battery& battery, bool isLoadOnBool){
	uint16_t voltage_mV = static_cast<uint16_t>(1000 * measurements.voltage);
	uint16_t current_mA = static_cast<uint16_t>(1000 * measurements.current);
	uint16_t temperature = static_cast<uint16_t>(measurements.temperature);
	uint16_t cutoffVoltage_mV = static_cast<uint16_t>(1000 * battery.cutoffVoltage.value);
	uint16_t dischargeCurrent_mA = static_cast<uint16_t>(1000 * battery.dischargeCurrent.value);
	uint16_t isLoadOn = static_cast<uint16_t>(isLoadOnBool);
	long totalSeconds = measurements.timer.getTotalSeconds();

	if(!isLoadOn)
		current_mA = 0;

	uint8_t message[6 * sizeof(uint16_t) + sizeof(long)];
	size_t index = 0;
	memcpy(&message[index], &voltage_mV, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(&message[index], &current_mA, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(&message[index], &temperature, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(&message[index], &cutoffVoltage_mV, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(&message[index], &dischargeCurrent_mA, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(&message[index], &isLoadOn, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(&message[index], &totalSeconds, sizeof(long));

	uint32_t crc = calculateCRC(message, sizeof(message));

	Serial.print("m;");
	Serial.print(voltage_mV);
	Serial.print(";");
	Serial.print(current_mA);
	Serial.print(";");
	Serial.print(temperature);
	Serial.print(";");
	Serial.print(cutoffVoltage_mV);
	Serial.print(";");
	Serial.print(dischargeCurrent_mA);
	Serial.print(";");
	Serial.print(isLoadOn);
	Serial.print(";");
	Serial.print(totalSeconds);
	Serial.print(";");
	Serial.println(crc);
}

int batteryCapacityMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls, Battery& battery){
	float prevSetCurrent = battery.dischargeCurrent.value;
	float prevCapacity = 0;
	uint32_t prevTime = 0;
	char data;
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("  Battery capacity  ");
  lcd.setCursor(0, 1);
	lcd.print("  measurement mode  ");
	lcd.setCursor(0, 2);
  lcd.print("Enter cutoff voltage");
	userInput.cursorPosX = 2;
	userInput.decimalPlace = ones;
	battery.changedVariable = DischargeCurrent;

	do{
		userInput.key = keypad.getKey();
		userInput.inputFromKeypad(lcd, battery.cutoffVoltage, 14, 2);
	} while (userInput.key != Enter);
  
	lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("I=");
	battery.dischargeCurrent.display(lcd);
	lcd.print("A  Off=");
	battery.cutoffVoltage.display(lcd);
	lcd.print("V");

	controls.loadOff(lcd);
	controls.regulateCurrent(battery.dischargeCurrent.value);

	while(1){
		if(Serial.available()>0){
			data = Serial.read();
    	if(data == 'o')
				controls.loadOnOffToggle(lcd);
			if(controls.isLoadOn())
				measurements.timer.start();
			else
				measurements.timer.stop();

			if(data == 'a'){
				userInput.setCurrent.value = Serial.parseFloat();
				userInput.setCurrent.limit();
				battery.dischargeCurrent.value = userInput.setCurrent.value;
				controls.regulateCurrent(userInput.setCurrent.value);
				lcd.setCursor(2, 2);
				battery.dischargeCurrent.display(lcd);

				if(prevSetCurrent != battery.dischargeCurrent.value){	//if changed discharge current
					prevCapacity = battery.capacity;
					prevTime = measurements.timer.getTotalSeconds();
					prevSetCurrent = battery.dischargeCurrent.value;
				}
			}
			if(data == 'c'){
				battery.cutoffVoltage.value = Serial.parseFloat();
				battery.cutoffVoltage.limit();
				lcd.setCursor(14, 2);
				battery.cutoffVoltage.display(lcd);
			}
			if(data == 'r'){
				measurements.timer.stop();
				measurements.timer.reset();
				battery.capacity = 0;
				prevCapacity = 0;
				prevTime = 0;
				if(controls.isLoadOn())
					measurements.timer.start();
			}
		}

		measurements.update();

		sendLoadStatisticsOverSerial(measurements, battery, controls.isLoadOn());

		measurements.displayMeasurements(lcd, controls.isLoadOn());
		controls.fanControll();
		lcd.setCursor(7, 3);
		lcd.print(measurements.timer.getTime());
		if (controls.isLoadOn()){
      battery.capacity = prevCapacity + (measurements.current * 1000) * ((measurements.timer.getTotalSeconds() - prevTime) / 3600.0);
    }
		battery.displayCapacity(lcd);
		if(measurements.voltage < battery.cutoffVoltage.value){
			controls.loadOff(lcd);
			measurements.timer.stop();
		}

		switch (keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				measurements.timer.stop();
				return 0; //exit this loop, go back to menu
				break;
			case LoadOnOff:
				controls.loadOnOffToggle(lcd);
				if(controls.isLoadOn())
					measurements.timer.start();
				else
					measurements.timer.stop();
				break;
			case Delete:
				measurements.timer.stop();
				measurements.timer.reset();
				battery.capacity = 0;
				prevCapacity = 0;
				prevTime = 0;
				if(controls.isLoadOn())
					measurements.timer.start();
				break;
			case Enter:
				encoder.reset();
				userInput.time = millis();
				while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
					measurements.update();

					sendLoadStatisticsOverSerial(measurements, battery, controls.isLoadOn());
					
					measurements.displayMeasurements(lcd, controls.isLoadOn());
					controls.fanControll();
					lcd.setCursor(7, 3);
					lcd.print(measurements.timer.getTime());
					if (controls.isLoadOn()){
						battery.capacity = prevCapacity + (measurements.current * 1000) * ((measurements.timer.getTotalSeconds() - prevTime) / 3600.0);
					}
					battery.displayCapacity(lcd);
					if(measurements.voltage < battery.cutoffVoltage.value){
						controls.loadOff(lcd);
						measurements.timer.stop();
					}
					controls.regulateCurrent(battery.dischargeCurrent.value);

					userInput.key = keypad.getKey();
					lcd.cursor();
					lcd.setCursor(userInput.cursorPosX,2);
					delay(100);
					if(userInput.key == Menu){
						controls.loadOff(lcd);
						measurements.timer.stop();
						return 0; //exit this loop, go back to menu
					}
					else if(userInput.key == LoadOnOff){
						controls.loadOnOffToggle(lcd);
						if(controls.isLoadOn())
							measurements.timer.start();
						else
							measurements.timer.stop();
					}

					switch (battery.changedVariable){
						case DischargeCurrent:
							userInput.inputFromKeypad(lcd, battery.dischargeCurrent, 2, 2);
							userInput.checkEncoder(lcd, battery.dischargeCurrent, encoder, 2, 2);
							break;
						case CutoffVoltage:
							userInput.inputFromKeypad(lcd, battery.cutoffVoltage, 14, 2);
							userInput.checkEncoder(lcd, battery.cutoffVoltage, encoder, 14, 2);
							break;
					}

					if(prevSetCurrent != battery.dischargeCurrent.value){	//if changed discharge current
						prevCapacity = battery.capacity;
						prevTime = measurements.timer.getTotalSeconds();
						prevSetCurrent = battery.dischargeCurrent.value;
					}

					if(userInput.key == '#'){	//toggle changed variable between dischargeCurrent and cutoffVoltage
						userInput.time = millis();
						switch (battery.changedVariable){
							case DischargeCurrent:
								userInput.cursorPosX = 15;
								userInput.decimalPlace = ones;
								battery.changedVariable = CutoffVoltage;
								break;
							
							case CutoffVoltage:
								userInput.cursorPosX = 2;
								userInput.decimalPlace = ones;
								battery.changedVariable = DischargeCurrent;
								break;
						}
					}
	
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
int taskLoop(ModeOfOperation mode, SetValue& setParameter, LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	while(1){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		controls.fanControll();
		loadControl(controls, userInput, mode);
		switch (keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				userInput.resetKeypadInput();
				return 0; //exit this loop, go back to menu
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
					lcd.setCursor(userInput.cursorPosX,2);
					delay(100);
					if(userInput.key == Menu){
						controls.loadOff(lcd);
						userInput.resetKeypadInput();
						return 0; //exit this loop, go back to menu
					}
					else if(userInput.key == LoadOnOff)
						controls.loadOnOffToggle(lcd);

					userInput.inputFromKeypad(lcd, setParameter, 6, 2);
					userInput.checkEncoder(lcd, setParameter, encoder, 6, 2);
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
	// 	measured voltage offset calibration	    //
	//////////////////////////////////////////////
	lcd.setCursor(0, 0);
	lcd.print("Calibrate voltage   ");
	lcd.setCursor(0, 2);
	lcd.print("cal offset=");
	lastCalibrationValue = measurements.calibration.getVoltageOffset();
	encoder.reset();
	while(!encoder.wasButtonPressed()){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setVoltageOffset(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(11,2);
		lcd.print(measurements.calibration.getVoltageOffset());
		lcd.print("      ");
		delay(100);
	}

	//////////////////////////////////////////////
	// measured voltage multiplier calibration	//
	//////////////////////////////////////////////
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");
	lastCalibrationValue = measurements.calibration.getVoltageMultiplier();
	encoder.reset();
	while(!encoder.wasButtonPressed()){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setVoltageMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15,2);
		lcd.print(measurements.calibration.getVoltageMultiplier());
		lcd.print("  ");
		delay(100);
	}

	//////////////////////////////////////////////
	// measured current offset calibration		//
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
	controls.dac.setVoltage(4095, false);		//draw as much current as possible

	lcd.setCursor(0, 0);
	lcd.print("Calibrate current   ");
	lcd.setCursor(0, 2);
	lcd.print("cal offset=");
	
	lastCalibrationValue = measurements.calibration.getCurrentOffset();
	encoder.reset();
	while(!encoder.wasButtonPressed()){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setCurrentOffset(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(11, 2);
		lcd.print(measurements.calibration.getCurrentOffset());
		lcd.print("      ");
		delay(100);
	}

	//////////////////////////////////////////////
	// measured current multiplier calibration	//
	//////////////////////////////////////////////
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");

	lastCalibrationValue = measurements.calibration.getCurrentMultiplier();
	encoder.reset();
	while(!encoder.wasButtonPressed()){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		measurements.calibration.setCurrentMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15, 2);
		lcd.print(measurements.calibration.getCurrentMultiplier());
		lcd.print("  ");
		delay(100);
	}

	

	//////////////////////////////////////////////
	// 		set current offset calibration		//
	//////////////////////////////////////////////
	lcd.setCursor(0, 0);
	lcd.print("Calibrate setCurrent");
	lcd.setCursor(0, 2);
	lcd.print("cal offset=");
	lastCalibrationValue = controls.calibration.getSetCurrentOffset();
	encoder.reset();
	while(!encoder.wasButtonPressed()){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		userInput.key = keypad.getKey();
		userInput.inputFromKeypad(lcd, userInput.setCurrent, 11, 2);
		controls.regulateCurrent(userInput.setCurrent.value);
		controls.calibration.setSetCurrentOffset(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(11, 2);
		lcd.print(controls.calibration.getSetCurrentOffset());
		lcd.print("      ");
		delay(100);
	}

	//////////////////////////////////////////////
	// 	set current multiplier calibration		//
	//////////////////////////////////////////////
	lcd.setCursor(0, 2);
	lcd.print("cal multiplier=");
	lastCalibrationValue = controls.calibration.getSetCurrentMultiplier();
	encoder.reset();
	while(!encoder.wasButtonPressed()){
		measurements.update();
		measurements.displayMeasurements(lcd, controls.isLoadOn());
		userInput.key = keypad.getKey();
		userInput.inputFromKeypad(lcd, userInput.setCurrent, 15, 2);
		controls.regulateCurrent(userInput.setCurrent.value);
		controls.calibration.setSetCurrentMultiplier(lastCalibrationValue + encoder.rotation());
		lcd.setCursor(15, 2);
		lcd.print(controls.calibration.getSetCurrentMultiplier());
		lcd.print("  ");
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

	userInput.resetKeypadInput();
	controls.loadOff(lcd);
}
