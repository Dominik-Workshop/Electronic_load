/**
 * @file encoder.cpp
 * @author Dominik Workshop
 * @brief registers encoder's rotation and based on that increments or decrements a counter, 
 *  detects button press (uses interrupts for both)
 * @version 0.1
 * @date 2022-08-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "encoder.hh"

Encoder * Encoder::instance;

/**
 * @brief initializes the Encoder, attaches interrupts to appropriate pins
 * 
 */
void Encoder::begin(){
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), isrRotation, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrButton, FALLING);
  instance = this;
}

/**
 * @brief resets both the counter that indicates rotation and a flag that indicates that the Encoder's button was pressed
 * 
 */
void Encoder::reset(){
  counter = 0;
  buttonPressed = false;
}

void Encoder::isrRotation(){
  instance->handleInterruptRotation();
}

void Encoder::isrButton(){
  instance->handleInterruptButton();
}

void Encoder::handleInterruptRotation(){
  static unsigned long lastInterruptTimeRotation = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if(interruptTime - lastInterruptTimeRotation > 5){
    if(digitalRead(ENCODER_B) == LOW) 
      counter--;
    else 
      counter++;
  }

  lastInterruptTimeRotation = interruptTime;
}

void Encoder::handleInterruptButton(){
  static unsigned long lastInterruptTimeButton = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 100ms, assume it's a bounce and ignore
  if(interruptTime - lastInterruptTimeButton > 100)
    buttonPressed = true;
  
  lastInterruptTimeButton = interruptTime;
}

/**
 * @brief checks if the button was pressed since last time it was checked 
 *  or since last Encoder::reset()
 * 
 * @return true when button was pressed
 * @return false when button wasn't pressed
 */
bool Encoder::wasButtonPressed(){
  if(buttonPressed){
    buttonPressed = false; //reset buttonPressed flag
    return true;
  }
  else
    return false;
}

/**
 * @brief checks how many steps was the encoder rotated since last Encoder::reset()
 * 
 * @return int number of steps, the value increases if rotated clockwise, decreases if rotated anticlockwise
 */
int Encoder::rotation(){
  return counter;
}