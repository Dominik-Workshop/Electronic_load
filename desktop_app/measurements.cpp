/**
 * @file measurements.cpp
 * @author Dominik Workshop
 * @brief
 * @version 1.0
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>

#include "measurements.h"

Measurements::Measurements(){
    voltageReadings = nullptr;
    currentReadings = nullptr;
    numberOfReadings = 0;
    arrayCapacity = 10;

    mAhCapacity = 0;
    WhCapacity = 0;
}

Measurements::~Measurements(){
    free(voltageReadings);
    free(currentReadings);
}

/**
 * @brief Adds voltage and current readings to the arrays
 * @param voltage
 * @param current
 */
void Measurements::addReadings(float voltage, float current){
    if (numberOfReadings >= arrayCapacity)
        arrayCapacity += 10;

    voltageReadings = (float*)realloc(voltageReadings, (arrayCapacity * sizeof(float)));
    currentReadings = (float*)realloc(currentReadings, (arrayCapacity * sizeof(float)));
    voltageReadings[numberOfReadings] = voltage;
    currentReadings[numberOfReadings] = current;
    ++numberOfReadings;
}

/**
 * @brief Resets the measurement data arrays and capacity values
 */
void Measurements::resetMeasurements(){
    free(voltageReadings);
    free(currentReadings);
    voltageReadings = nullptr;
    currentReadings = nullptr;
    numberOfReadings = 0;
    arrayCapacity = 10;

    mAhCapacity = 0;
    WhCapacity = 0;
}
