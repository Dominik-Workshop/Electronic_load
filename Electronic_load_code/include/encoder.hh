/**
 * @file encoder.hh
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ENCODER_HH
#define ENCODER_HH

#include <Arduino.h>
#include "defines.hh"

volatile int EncoderValue = 0;

void isr(){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if(interruptTime - lastInterruptTime > 5){
    if(digitalRead(ENCODER_B) == LOW) 
      EncoderValue++;
    else 
      EncoderValue--;
  }
  
  lastInterruptTime = interruptTime;
}

#endif