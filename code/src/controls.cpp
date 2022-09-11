/**
 * @file controls.cpp
 * @author Dominik Workshop
 * @brief fan, load on/off, current/power/resitance regulation controlls
 * @version 1.1
 * @date 2022-09-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "controls.hh"

/**
 * @brief initializes DAC and reads calibration values from EEPROM when the object is created
 * 
 */
Controls::Controls(Measurements& measurements_): measurements(measurements_){
	dac.begin(DAC_ADDRESS);
	calibration.readFromEEPROM();
}

/**
 * @brief controls the fan speed depending on measured temperature
 * (with hysteresis)
 * 
 */
void Controls::fanControll(){
	fanspeed = map(measurements.temperature, lowTemperatureThreshold, 45, 60, 255);
	//hysteresis, so that fans don't turn on/off repeatedly when crossing the threshold
  if (measurements.temperature < lowTemperatureThreshold){
    fanspeed = 0;
    lowTemperatureThreshold = 31;
  }
  if (measurements.temperature >= lowTemperatureThreshold)
     lowTemperatureThreshold=30;

  //if it's hot drive fans at full speed
  if (measurements.temperature > 45){
    fanspeed = 255;
	}
	analogWrite(FAN, fanspeed);
}

/**
 * @brief turns the load on and displays "ON " in the upper right corner of the lcd
 * 
 * @param lcd 
 */
void Controls::loadOn(LiquidCrystal_I2C& lcd){
	digitalWrite(OUTPUT_OFF, HIGH);
	lcd.setCursor(17, 0);
	lcd.print("ON ");
	loadIsOn = true;
}

/**
 * @brief turns the load off and displays "OFF" in the upper right corner of the lcd
 * 
 * @param lcd 
 */
void Controls::loadOff(LiquidCrystal_I2C& lcd){
	digitalWrite(OUTPUT_OFF, LOW);
	lcd.setCursor(17, 0);
	lcd.print("OFF");
	loadIsOn = false;
}

/**
 * @brief toggles the load on or off and displays "ON " or "OFF" in the upper right corener of the lcd
 * 
 * @param lcd 
 */
void Controls::loadOnOffToggle(LiquidCrystal_I2C& lcd){
	if(loadIsOn){
		loadOff(lcd);
	}
	else if(!loadIsOn){
		loadOn(lcd);
	}	
}

void Controls::regulateCurrent(float setCurrent){
	if(setCurrent > MAX_CURRENT)
		setCurrent = MAX_CURRENT;
	if(setCurrent * measurements.voltage > MAX_POWER)
		setCurrent = MAX_POWER / measurements.voltage;

								//setCurrent*resolution/Vref*attenuation/equivalent R of shunts * calibrationMultiplier + calibrationOffset
	int dacControl = setCurrent * 4096 / 4.096 / 0.053 * 0.025 * (0.95 + calibration.getSetCurrentMultiplier()/2550.0) + (calibration.getSetCurrentOffset() - 128);
	if(dacControl < 0)						//////////////////////////////
		dacControl = 0;							//	ensure that dacControl	//
	else if(dacControl > 4095)		//	is between 0 and 4095		//
		dacControl = 4095;					//////////////////////////////
	dac.setVoltage(dacControl, false);
}

void Controls::regulatePower(float setPower){
	regulateCurrent(setPower/measurements.voltage);
}

void Controls::regulateResistance(float setResistance){
	regulateCurrent(measurements.voltage/setResistance);
}

bool Controls::isLoadOn(){
	return loadIsOn;
}