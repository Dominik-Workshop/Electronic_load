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
#include "set_value.hh"
#include "controls.hh"

enum TransientChangedVariable{
  LowCurrent,
  HighCurrent,
  PulseTime
};

class Transient{
  public:
    SetValue lowCurrent;
    SetValue highCurrent;
    SetValue pulseTime; //in ms
    TransientChangedVariable changedVariable;

    void toggleCurrent(Controls& controls);

    Transient();
  private:
    bool lastCurrent = 0; //0 - lowCurrent, 1 - highCurrent
};

#endif