/**
 * @file moving_average_filter.hh
 * @author Dominik Workshop
 * @brief 
 * @version 1.0
 * @date 2022-08-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOVING_AVERAGE_FILTER_HH
#define MOVING_AVERAGE_FILTER_HH


/**
 * @brief digital low pass filter
 * 
 * @tparam T type of averaged values
 * @tparam num_of_readings 
 */
template<typename T, int num_of_readings>
class MovingAverage{
  private:
    T readings[num_of_readings];   //stores defined amount of readings to be averaged out
    int readIndex = 0;                 //the index of the current reading
    T total = 0;                   //sum of all readings
    T average = 0;
  public:
    MovingAverage();
    T calculate(T reading);
    T getAverage();
};

/**
 * @brief zero out all numbers stored in the array
 * 
 */
template<typename T, int num_of_readings>
MovingAverage<T, num_of_readings>::MovingAverage(){
  for (int i = 0; i < num_of_readings; ++i)
    readings[i] = 0;
}

/**
 * @brief adds new reading to the moving average filter and calculates new average value
 * 
 * @param reading value to be averaged out
 * @return int current average value of passed readings
 */
template<typename T, int num_of_readings>
T MovingAverage<T, num_of_readings>::calculate(T reading){
      total -= readings[readIndex];       //subtract the last reading:
      readings[readIndex] = reading;      //read from the sensor:
      total += readings[readIndex];       //add the reading to the total:
      ++readIndex;                        //advance to the next position in the array:
      if (readIndex >= num_of_readings)   //if we're at the end of the array...
        readIndex = 0;                    //...wrap around to the beginning:
      average = total / num_of_readings;  //calculate the average:

      return average;
}

/**
 * @brief gives acces to current average value
 * 
 * @return int current average value
 */
template<typename T, int num_of_readings>
T MovingAverage<T, num_of_readings>::getAverage(){
  return average;
}

#endif