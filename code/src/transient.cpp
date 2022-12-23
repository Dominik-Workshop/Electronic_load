/**
 * @file transient.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "transient.hh"

Transient::Transient(){
  lowCurrent.init(4, 3, 0.004, MAX_CURRENT, 0.01);
  highCurrent.init(4, 3, 0.004, MAX_CURRENT, 0.1);
  pulseTime.init(4, 0, 1, 9999, 50);
}

void Transient::toggleCurrent(Controls& controls){
  if(lastCurrent == 0){
    controls.regulateCurrent(highCurrent.value);
    lastCurrent = 1;
  }
  else{
    controls.regulateCurrent(lowCurrent.value);
    lastCurrent = 0;
  }
}

/**
 * @brief constantly toggles between low and high current
 * 
 * @param lcd 
 * @param userInput 
 * @param keypad 
 * @param encoder 
 * @param measurements 
 * @param controls 
 * @return int 
 */
int Transient::continousMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
  lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Time=");
	pulseTime.display(lcd);
	lcd.print("ms      OFF");
  lcd.setCursor(0, 2);
  lcd.print("Lo=");
	lowCurrent.display(lcd);
	lcd.print("A  Hi=");
	highCurrent.display(lcd);
	lcd.print("A");

  userInput.cursorPosX = 3;
  userInput.cursorPosY = 2;
  userInput.decimalPlace = ones;
  changedVariable = LowCurrent;
  lastTime = millis();
  while(1){
    measurements.update();
    measurements.displayMeasurements(lcd, controls.isLoadOn());
    controls.fanControll();
    if(lastTime + pulseTime.value <= millis()){	//toggle current after set time
      toggleCurrent(controls);
      lastTime = millis();
    }

    switch (keypad.getKey()){
      case Menu:
        controls.loadOff(lcd);
        userInput.resetKeypadInput();
        return 0; //exit this loop, go back to menu
        break;
      case LoadOnOff:
        lastTime = millis();
        controls.loadOnOffToggle(lcd);
        break;
      case Enter:
        controls.loadOff(lcd);
        encoder.reset();
        userInput.time = millis();
        while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
          measurements.update();
          measurements.displayMeasurements(lcd, controls.isLoadOn());
          controls.fanControll();

          userInput.key = keypad.getKey();
          lcd.cursor();
          lcd.setCursor(userInput.cursorPosX, userInput.cursorPosY);
          delay(100);
          if(userInput.key == Menu){
            controls.loadOff(lcd);
            userInput.resetKeypadInput();
            return 0; //exit this loop, go back to menu
          }
          else if(userInput.key == LoadOnOff)
            controls.loadOnOffToggle(lcd);

          switch (changedVariable){
            case LowCurrent:
              userInput.inputFromKeypad(lcd, lowCurrent, 3, 2);
              userInput.checkEncoder(lcd, lowCurrent, encoder, 3, 2);
              break;
            case HighCurrent:
              userInput.inputFromKeypad(lcd, highCurrent, 14, 2);
              userInput.checkEncoder(lcd, highCurrent, encoder, 14, 2);
              break;
            case PulseTime:
              userInput.inputFromKeypad(lcd, pulseTime, 5, 0);
              userInput.checkEncoder(lcd, pulseTime, encoder, 5, 0);
              break;
          }

          if(userInput.key == '#'){	//toggle changed variable between LowCurrent, HighCurrent and PulseTime
            userInput.time = millis();
            switch (changedVariable){
              case LowCurrent:
                userInput.cursorPosX = 14;
                userInput.decimalPlace = ones;
                changedVariable = HighCurrent;
                break;
              case HighCurrent:
                userInput.cursorPosX = 8;
                userInput.cursorPosY = 0;
                userInput.decimalPlace = ones;
                changedVariable = PulseTime;
                break;
              case PulseTime:
                userInput.cursorPosX = 3;
                userInput.cursorPosY = 2;
                userInput.decimalPlace = ones;
                changedVariable = LowCurrent;
                break;
              default:
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
 * @brief toggles from low to high current for set amount of time, after triggered (press "." on the keypad)
 * 
 * @param lcd 
 * @param userInput 
 * @param keypad 
 * @param encoder 
 * @param measurements 
 * @param controls 
 * @return int 
 */
int Transient::pulseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
  lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Time=");
	pulseTime.display(lcd);
	lcd.print("ms      OFF");
  lcd.setCursor(0, 2);
  lcd.print("Lo=");
	lowCurrent.display(lcd);
	lcd.print("A  Hi=");
	highCurrent.display(lcd);
	lcd.print("A");

  userInput.cursorPosX = 3;
  userInput.cursorPosY = 2;
  userInput.decimalPlace = ones;
  changedVariable = LowCurrent;

  controls.regulateCurrent(lowCurrent.value);
  while(1){
    measurements.update();
    measurements.displayMeasurements(lcd, controls.isLoadOn());
    controls.fanControll();
    userInput.key = keypad.getKey();
    if(userInput.key == '.' && controls.isLoadOn()){	//pulse current after . is pressed on keypad
      lastTime = millis();
      controls.regulateCurrent(highCurrent.value);
      while(lastTime + pulseTime.value > millis()){
        measurements.update();
        measurements.displayMeasurements(lcd, controls.isLoadOn());
        controls.fanControll();
      }
      controls.regulateCurrent(lowCurrent.value);
    }

    switch (userInput.key){
      case Menu:
        controls.loadOff(lcd);
        userInput.resetKeypadInput();
        return 0; //exit this loop, go back to menu
        break;
      case LoadOnOff:
        lastTime = millis();
        controls.loadOnOffToggle(lcd);
        break;
      case Enter:
        encoder.reset();
        userInput.time = millis();
        while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
          measurements.update();
          measurements.displayMeasurements(lcd, controls.isLoadOn());
          controls.fanControll();

          userInput.key = keypad.getKey();
          lcd.cursor();
          lcd.setCursor(userInput.cursorPosX, userInput.cursorPosY);
          delay(100);
          if(userInput.key == Menu){
            controls.loadOff(lcd);
            userInput.resetKeypadInput();
            return 0; //exit this loop, go back to menu
          }
          else if(userInput.key == LoadOnOff)
            controls.loadOnOffToggle(lcd);

          switch (changedVariable){
            case LowCurrent:
              userInput.inputFromKeypad(lcd, lowCurrent, 3, 2);
              userInput.checkEncoder(lcd, lowCurrent, encoder, 3, 2);
              break;
            case HighCurrent:
              userInput.inputFromKeypad(lcd, highCurrent, 14, 2);
              userInput.checkEncoder(lcd, highCurrent, encoder, 14, 2);
              break;
            case PulseTime:
              userInput.inputFromKeypad(lcd, pulseTime, 5, 0);
              userInput.checkEncoder(lcd, pulseTime, encoder, 5, 0);
              break;
          }

          if(userInput.key == '#'){	//toggle changed variable between LowCurrent, HighCurrent and PulseTime
            userInput.time = millis();
            switch (changedVariable){
              case LowCurrent:
                userInput.cursorPosX = 14;
                userInput.decimalPlace = ones;
                changedVariable = HighCurrent;
                break;
              case HighCurrent:
                userInput.cursorPosX = 8;
                userInput.cursorPosY = 0;
                userInput.decimalPlace = ones;
                changedVariable = PulseTime;
                break;
              case PulseTime:
                userInput.cursorPosX = 3;
                userInput.cursorPosY = 2;
                userInput.decimalPlace = ones;
                changedVariable = LowCurrent;
                break;
              default:
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
int Transient::toggleMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
  lcd.clear();
	lcd.setCursor(17, 0);
	lcd.print("OFF");
  lcd.setCursor(0, 2);
  lcd.print("Lo=");
	lowCurrent.display(lcd);
	lcd.print("A  Hi=");
	highCurrent.display(lcd);
	lcd.print("A");

  userInput.cursorPosX = 3;
  userInput.cursorPosY = 2;
  userInput.decimalPlace = ones;
  changedVariable = LowCurrent;

  controls.regulateCurrent(lowCurrent.value);
  while(1){
    measurements.update();
    measurements.displayMeasurements(lcd, controls.isLoadOn());
    controls.fanControll();
    userInput.key = keypad.getKey();
    if(userInput.key == '.'){	//toggle current after . is pressed on keypad
      toggleCurrent(controls);
    }

    switch (userInput.key){
      case Menu:
        controls.loadOff(lcd);
        userInput.resetKeypadInput();
        return 0; //exit this loop, go back to menu
      case LoadOnOff:
        lastTime = millis();
        controls.loadOnOffToggle(lcd);
        break;
      case Enter:
        controls.loadOff(lcd);
        encoder.reset();
        userInput.time = millis();
        while(userInput.time + 5000 > millis()){  //exit after 5s of inactivity
          measurements.update();
          measurements.displayMeasurements(lcd, controls.isLoadOn());
          controls.fanControll();

          userInput.key = keypad.getKey();
          lcd.cursor();
          lcd.setCursor(userInput.cursorPosX, userInput.cursorPosY);
          delay(100);
          if(userInput.key == Menu){
            controls.loadOff(lcd);
            userInput.resetKeypadInput();
            return 0; //exit this loop, go back to menu
          }
          else if(userInput.key == LoadOnOff)
            controls.loadOnOffToggle(lcd);

          switch (changedVariable){
            case LowCurrent:
              userInput.inputFromKeypad(lcd, lowCurrent, 3, 2);
              userInput.checkEncoder(lcd, lowCurrent, encoder, 3, 2);
              break;
            case HighCurrent:
              userInput.inputFromKeypad(lcd, highCurrent, 14, 2);
              userInput.checkEncoder(lcd, highCurrent, encoder, 14, 2);
              break;
          }

          if(userInput.key == '#'){	//toggle changed variable between LowCurrent and HighCurrent
            userInput.time = millis();
            switch (changedVariable){
              case LowCurrent:
                userInput.cursorPosX = 14;
                userInput.decimalPlace = ones;
                changedVariable = HighCurrent;
                break;
              case HighCurrent:
                userInput.cursorPosX = 3;
                userInput.cursorPosY = 2;
                userInput.decimalPlace = ones;
                changedVariable = LowCurrent;
                break;
              default:
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