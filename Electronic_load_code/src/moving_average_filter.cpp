/**
 * @file moving_average_filter.cpp
 * @author Dominik Workshop
 * @brief 
 * @version 0.1
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "moving_average_filter.hh"

/**
 * @brief zero out all numbers stored in the array
 * 
 */
MovingAverage::MovingAverage(){
  for (int i = 0; i < NUM_OF_READINGS; ++i)
    readings[i] = 0;
}

/**
 * @brief adds new reading to the moving average filter and calculates new average value
 * 
 * @param reading value to be averaged out
 * @return int current average value of passed readings
 */
int MovingAverage::calculate(int reading){
      total -= readings[readIndex];       //subtract the last reading:
      readings[readIndex] = reading;      //read from the sensor:
      total += readings[readIndex];       //add the reading to the total:
      ++readIndex;                        //advance to the next position in the array:
      if (readIndex >= NUM_OF_READINGS)   //if we're at the end of the array...
        readIndex = 0;                    //...wrap around to the beginning:
      average = total / NUM_OF_READINGS;  //calculate the average:

      return average;
    }

/**
 * @brief gives acces to current average value
 * 
 * @return int current average value
 */
int MovingAverage::getAverage(){
  return average;
}