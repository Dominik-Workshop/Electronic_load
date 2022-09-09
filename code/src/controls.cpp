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

int fanLowThreshold = 30;
int prev = 1;

void fanControll(Measurements& measurements){
	int fanspeed;
	fanspeed = map(measurements.temperature, fanLowThreshold, 45, 60, 255);
	//hysteresis, so that fans don't turn on/off repeatedly when crossing the threshold
  if (measurements.temperature < fanLowThreshold){
    fanspeed = 0;
    fanLowThreshold = 31;
  }
  if (measurements.temperature >= fanLowThreshold){
     fanLowThreshold=30;
  }
  //if it's hot drive fans at full speed
  if (measurements.temperature > 45){
    fanspeed = 255;
	}
	analogWrite(FAN, fanspeed);
}

void loadOnOffToggle(LiquidCrystal_I2C& lcd){
	if(prev == 0){
		digitalWrite(OUTPUT_OFF, LOW);
		lcd.setCursor(17, 0);
		lcd.print("OFF");
		prev = 1;
	}
	else if(prev == 1){
		digitalWrite(OUTPUT_OFF, HIGH);
		lcd.setCursor(17, 0);
		lcd.print("ON ");
		prev = 0;
	}	
}

void sinkCurrent(float current, Adafruit_MCP4725& dac){
	dac.setVoltage(current * 275, false);
}