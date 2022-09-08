/**
 * @file measurements.cpp
 * @author Dominik Workshop
 * @brief takes voltage and current measurements, calculates power, stores these values, can display all the measurements on the lcd
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "measurements.hh"

Measurements::Measurements(){
  calibration.readFromEEPROM();
}

/**
 * @brief make votage, current and temperature readings, calculate power and store these values
 * 
 * @param adc 
 */
void Measurements::update(Adafruit_ADS1115& adc){
  measureVoltage(adc);
  measureCurrent(adc);
  measureTemperature();
  power = voltage * current;
}

/**
 * @brief displays voltage, current, power and temperature
 * 
 * @param lcd 
 */
void Measurements::displayMeasurements(LiquidCrystal_I2C& lcd){
  lcd.setCursor(0,1);
  display(lcd, voltage, 4, 2);
  lcd.print("V ");
  display(lcd, current, 4, 3);
  lcd.print("A ");
  display(lcd, power, 4, 1);
  lcd.print("W");
  lcd.setCursor(16,3);
	lcd.print(temperature);
	lcd.write(degree);
	lcd.print("C");
}

float Measurements::measureVoltage(Adafruit_ADS1115& adc){
  adc.setGain(GAIN_EIGHT);  // 8x gain   +/- 0.512V  1 bit = 0.015625mV
            //        reading_from_adc / resolution * Vmax * attenuation
  voltage = ((adc.readADC_Differential_2_3()/ 32768.0) * 0.512 * 100) * (0.95 + calibration.voltageMultiplier/2550.0);
  noLessThanZero(voltage);
  return voltage;
}

float Measurements::measureCurrent(Adafruit_ADS1115& adc){
  adc.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
            //      reading_from_adc / resolution * Vmax / R  * number of shunts
  current = (((adc.readADC_SingleEnded(0) / 32768.0) * 0.256 / 0.1) * 4) * (0.95 + calibration.currentMultiplier/2550.0) + calibration.currentOffset/25500.0;
  noLessThanZero(current);
  return current;
}

int Measurements::measureTemperature(){
  int Vo;
  float R2, logR2;
  Vo = analogRead(THERMISTOR);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  temperature = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  temperature = temperature - 273.15;
  return temperature;
}

/**
 * @brief displays given number with specified amount of digits and decimal places
 * 
 * @param lcd 
 * @param value to be displayed
 * @param numOfDigits 
 * @param numOfDecimalPlaces 
 */
void Measurements::display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces){
  char displayValue[10];
  dtostrf(value, (numOfDigits + 1), numOfDecimalPlaces, displayValue);
	lcd.print(displayValue);
}

/**
 * @brief if value is negative it becomes zero
 * 
 * @param value is limited to be only positive
 */
void Measurements::noLessThanZero(float& value){
  if(value < 0)
    value = 0;
}