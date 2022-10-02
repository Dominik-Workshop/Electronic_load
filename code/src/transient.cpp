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
  lowCurrent.value = 0.01;
  lowCurrent.init(4, 3, 0.004, MAX_CURRENT);
  highCurrent.value = 0.1;
  highCurrent.init(4, 3, 0.004, MAX_CURRENT);
  pulseTime.value = 50;
  pulseTime.init(4, 0, 1, 9999);
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