
/**
 * @file controls.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
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
Controls::Controls(){
	dac.begin(DAC_ADDRESS);
	calibration.readFromEEPROM();
}

void Controls::fanControll(Measurements& measurements){
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

void Controls::loadOn(LiquidCrystal_I2C& lcd){
	digitalWrite(OUTPUT_OFF, HIGH);
	lcd.setCursor(17, 0);
	lcd.print("ON ");
	loadIsOn = true;
}
void Controls::loadOff(LiquidCrystal_I2C& lcd){
	digitalWrite(OUTPUT_OFF, LOW);
	lcd.setCursor(17, 0);
	lcd.print("OFF");
	loadIsOn = false;
}

void Controls::loadOnOffToggle(LiquidCrystal_I2C& lcd){
	if(loadIsOn){
		loadOff(lcd);
	}
	else if(!loadIsOn){
		loadOn(lcd);
	}	
}

void Controls::sinkCurrent(float setCurrent){

								//setCurrent*resolution/Vref*attenuation/equivalent R of shunts * calibrationValue
	dac.setVoltage(setCurrent * 4096 / 4.096 / 0.053 * 0.025 * (0.95 + calibration.getSetCurrentMultiplier()/2550.0), false);
}

void Controls::drawConstPower(float setPower, float voltage){
	sinkCurrent(setPower/voltage);
}

void Controls::constResistance(float setResistance, float voltage){
	sinkCurrent(voltage/setResistance);
}