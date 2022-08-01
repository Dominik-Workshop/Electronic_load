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
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), isr, LOW);
    instance = this;
}

void Encoder::isr(){
    instance->handleInterrupt();
}

void Encoder::handleInterrupt(){
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

int Encoder::getCounts(){
  return counter;
}