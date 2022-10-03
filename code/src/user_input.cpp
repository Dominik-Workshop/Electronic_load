/**
 * @file user_input.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 1.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "user_input.hh"

UserInput::UserInput(){
  setCurrent.value = 0.1; //initialize set current to 0,1A at power up
  setCurrent.init(4, 3, 0.004, MAX_CURRENT);
  setPower.value = 1;  //initialize set power to 1W at power up
  setPower.init(5,2, 0.01, MAX_POWER);
  setResistance.value = 100;  //initialize set resitance to 100ohm at power up
  setResistance.init(5,1, 0.1, MAX_RESISTANCE);
}

/**
 * @brief manages input from keypad, displays entered digit or decimal point in the bottom left corner of the lcd,
 * when Enter pressed displays new value of setParameter on the lcd in position given by xPosiotion and yPosition
 * 
 * @param lcd 
 * @param setParameter 
 * @param xPositon horizontal positon on the lcd to display the most significant digit of setParameter
 * @param yPosition vertical positon on the lcd to display the setParameter
 */
void UserInput::inputFromKeypad(LiquidCrystal_I2C& lcd, SetValue& setParameter, int xPositon, int yPosition){
	if(key >= '0' && key <= '9'){	//is digit
		if(index <=5){	//limit number of input characters
			numbers[index] = key;
			numbers[++index] = '\0';
			lcd.setCursor(x_pos,3);                              
			lcd.print(key);  //show input number on the LCD
			x_pos++;
			time = millis();
		}
	}

	else if(key == '.' && !decimalPointPresent){
		if(index <=5){	//limit number of input characters
			numbers[index] = '.';
			numbers[++index] = '\0';
			lcd.setCursor(x_pos,3);
			lcd.print(".");
			x_pos++;
			decimalPointPresent = true;
			time = millis();
		}
	}
	
	else if(key == Delete){
		lcd.setCursor(--x_pos,3);
		lcd.print(" ");
		--index;
		if(numbers[index] == '.')		//deleted decimal point
			decimalPointPresent = false;
		numbers[index] = '\0';
		time = millis();
	}

	else if(key == Enter && index > 0) {
		setParameter.value = atof(numbers);		//convert array of entered numbers to float and write it to SetValue variable
		setParameter.limit();
		lcd.setCursor(xPositon, yPosition);
		setParameter.display(lcd);
		lcd.setCursor(0,3);
		lcd.print("       ");		//clear all entered numbers from the screen
    resetKeypadInput();
	}
}

void UserInput::resetKeypadInput(){
  for(int i = 0; i < 7; ++i)
    numbers[i] = '\0';
  decimalPointPresent = false;
  index = 0;
  x_pos = 0;
}

/**
 * @brief manages input from rotary encoder,
 * displays new value of setParameter on the lcd in position given by xPosiotion and yPosition, if encoder was rotated
 * 
 * @param lcd 
 * @param setParameter 
 * @param encoder 
 * @param xPositon horizontal positon on the lcd to display the most significant digit of setParameter
 * @param yPosition vertical positon on the lcd to display the setParameter
 */
void UserInput::checkEncoder(LiquidCrystal_I2C& lcd, SetValue& setParameter, Encoder& encoder, int xPositon, int yPosition){
	if(encoder.wasButtonPressed()){
		time = millis();
		if(decimalPlace > setParameter.minDecimalPlace){	//if did't reach the last digit of setParameter
			--decimalPlace;																	//move cursor to the left
			++cursorPosX;
		}   
		else{
			decimalPlace = setParameter.maxDecimalPlace;		//move the cursor back to the first digit of setParameter
			cursorPosX = xPositon;
		}
		if(decimalPlace == tenths)	//jump across the decimal point
			++cursorPosX;
	}

	if(encoder.rotation()){
		setParameter.value += encoder.rotation() * pow(10, decimalPlace);
		setParameter.limit();
		lcd.setCursor(xPositon, yPosition);
		setParameter.display(lcd);
		encoder.reset();
		time = millis();
	}
}