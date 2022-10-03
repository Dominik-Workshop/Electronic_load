/**
 * @file transient.hh
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TRANSIENT_HH
#define TRANSIENT_HH

#include "defines.hh"
#include "user_input.hh"
#include "controls.hh"

enum TransientChangedVariable{
  LowCurrent,
  HighCurrent,
  PulseTime
};

/**
 * @brief contains setValues: lowCurrent, highCurrent, pulseTime, information which setValue is currently changed,
 * can toggle the current betweet low and high current
 * 
 */
class Transient{
  public:
    SetValue lowCurrent;
    SetValue highCurrent;
    SetValue pulseTime; //in ms

    int continousMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
    int pulseMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);
    int toggleMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);

    Transient();
  private:
    bool lastCurrent = 0; //0 - lowCurrent, 1 - highCurrent
    uint32_t lastTime;	//used to time toggles between high and low currents
    TransientChangedVariable changedVariable;
    void toggleCurrent(Controls& controls);
};

#endif