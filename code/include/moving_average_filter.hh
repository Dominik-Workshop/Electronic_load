/**
 * @file moving_average_filter.hh
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOVING_AVERAGE_FILTER_HH
#define MOVING_AVERAGE_FILTER_HH

#include "defines.hh"

/**
 * @brief digital low pass filter
 * 
 */
class MovingAverage{
  private:
    float readings[NUM_OF_READINGS];   //stores defined amount of readings to be averaged out
    int readIndex = 0;                 //the index of the current reading
    float total = 0;                   //sum of all readings
    float average = 0;
  public:
    MovingAverage();
    int calculate(int reading);
    int getAverage();
};

#endif