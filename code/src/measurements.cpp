/**
 * @file measurements.cpp
 * @author Dominik Workshop
 * @brief takes voltage, current and temperature measurements,
 * calculates power, stores these values, can display all the measurements on the lcd
 * @version 1.0
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "measurements.hh"

/**
 * @brief initializes ADC and reads calibration values from EEPROM when the object is created
 * 
 */
Measurements::Measurements(){
  adc.begin(ADC_ADDRESS);
  calibration.readFromEEPROM();
}

/**
 * @brief make voltage, current and temperature readings, calculate power and store these values
 * 
 * @param adc 
 */
void Measurements::update(){
  measureVoltage();
  measureCurrent();
  measureTemperature();
  power = voltage * current;
}

/**
 * @brief displays voltage, current, power and temperature.
 * If load is off then sets current and power to zero to cancel out the current flowing through shunts
 * casued by turning on of Q8-Q11 transistors
 * 
 * @param lcd 
 * @param loadIsOn boolean value
 */
void Measurements::displayMeasurements(LiquidCrystal_I2C& lcd, bool loadIsOn){
  if(!loadIsOn){
    current = 0;
    power = 0;
  }
  lcd.setCursor(0,1);
  display(lcd, voltage, 4, 3);
  lcd.print("V ");
  display(lcd, current, 4, 3);
  lcd.print("A ");
  display(lcd, power, 4, 2);
  lcd.print("W");
  lcd.setCursor(16,3);
	lcd.print(temperature);
	lcd.write(degree);
	lcd.print("C");
}

/**
 * @brief measures voltage treating negative readings as zero, uses calibration values to correct the measurement
 * 
 * @param adc 
 * @return float voltage
 */
float Measurements::measureVoltage(){
  adc.setGain(GAIN_EIGHT);  // 8x gain   +/- 0.512V  1 bit = 0.015625mV
            //        reading_from_adc / resolution * Vmax * attenuation * (0.95 to 1.05)
  voltage = ((adc.readADC_Differential_2_3()/ 32768.0) * 0.512 * 100) * (0.95 + calibration.getVoltageMultiplier()/2550.0);
  noLessThanZero(voltage);
  return voltage;
}

/**
 * @brief measures current treating negative readings as zero, uses calibration values to correct the measurement 
 * 
 * @param adc 
 * @return float current
 */
float Measurements::measureCurrent(){
  adc.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
            //      reading_from_adc / resolution * Vmax / R  * number of shunts * (0.95 to 1.05) + (0 to 0.01)
  current = (((adc.readADC_SingleEnded(0) / 32768.0) * 0.256 / 0.1) * 4) * (0.95 + calibration.getCurrentMultiplier()/2550.0) + calibration.getCurrentOffset()/25500.0;
  noLessThanZero(current);
  return current;
}

/**
 * @brief measures temperature with NTC thermistor
 * 
 * @return int temperature
 */
int Measurements::measureTemperature(){
  int Vo;
  float R2, logR2;
  Vo = analogRead(THERMISTOR);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  temperature = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  temperature -= 273.15;
  return temperature;
}

/**
 * @brief displays given number with specified amount of digits and decimal places
 * 
 * @param lcd 
 * @param value to be displayed
 * @param numOfDigits number of all digits
 * @param numOfDecimalPlaces number of digits after decimal place
 */
void Measurements::display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces){
  char displayValue[10];

   //when value is bigger than expected decrease the resolution to fit on the display 
  if(value >= pow(10, (numOfDigits-numOfDecimalPlaces)))
    --numOfDecimalPlaces;

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