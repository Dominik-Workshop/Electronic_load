/**
 * @file init.cpp
 * @author Dominik Workshop
 * @brief initializes all used pins
 * @version 0.1
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "init.hh"

/**
 * @brief initializes input and output pins
 * 
 */
void pinInit(){
  pinMode(THERMISTOR, INPUT);
  pinMode(FAN, OUTPUT);
  pinMode(OUTPUT_OFF, OUTPUT);
  pinMode(IN_TRIGGER, INPUT);
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
}

