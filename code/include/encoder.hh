/**
 * @file encoder.hh
 * @author Dominik Workshop
 * @brief registers encoder's rotation and based on that increments or decrements a counter, 
 *  detects button press (uses interrupts for both)
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
  private:
    volatile int counter = 0;
    volatile bool buttonPressed = false;
    static Encoder * instance;
    static void isrRotation();
    static void isrButton();
    void handleInterruptRotation ();
    void handleInterruptButton ();

  public:
    void begin ();
    void reset();
    bool wasButtonPressed();
    int rotation();
};

#endif