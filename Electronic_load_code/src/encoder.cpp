/**
 * @file encoder.cpp
 * @author Dominik Workshop
 * @brief registers encoder's rotation and based on that increments or decrements a counter, 
 *  which value can be accessed via getCouts method
 * @version 0.1
 * @date 2022-08-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "encoder.hh"

Encoder * Encoder::instance;

void Encoder::begin(){
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), isrRotation, LOW);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrButton, FALLING);
    instance = this;
}

void Encoder::isrRotation(){
    instance->handleInterruptRotation();
}

void Encoder::isrButton(){
    instance->handleInterruptButton();
}

void Encoder::handleInterruptRotation(){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if(interruptTime - lastInterruptTime > 5){
    if(digitalRead(ENCODER_B) == LOW) 
      counter++;
    else 
      counter--;
  }
  
  lastInterruptTime = interruptTime;
}

void Encoder::handleInterruptButton(){
    buttonPressed = true;
}

int Encoder::getCounts(){
  return counter;
}

bool Encoder::wasButtonPressed(){
    if(buttonPressed){
        buttonPressed = false;
        return true;
    }
    else
        return false;
}