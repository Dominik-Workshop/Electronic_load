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

class Encoder{
  static void isr();
  static Encoder * instance;

  void handleInterrupt ();

  volatile int counter = 0;

  public:
    void begin ();
    int getCounts();
};

void Encoder::begin(){
    attachInterrupt(digitalPinToInterrupt(2), isr, LOW);
    instance = this;
}
void Encoder::isr()
  {
    instance->handleInterrupt();
  }

Encoder * Encoder::instance;

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

#endif