/**
 * @file temperature.cpp
 * @author Dominik Workshop
 * @brief calculates temperature from thermistor reading
 * @version 0.1
 * @date 2022-07-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "temperature.hh"


int measureTemperature(){
    int Vo, Temperature;
    float R2, logR2;
    Vo = analogRead(THERMISTOR);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    Temperature = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
    Temperature = Temperature - 273.15;
    return Temperature;
}