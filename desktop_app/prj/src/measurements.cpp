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
    mAhCapacity = 0;
    WhCapacity = 0;
    mAhNominalCapacity = 0;
}

Measurements::~Measurements(){
}

/**
 * @brief Adds voltage and current readings to the arrays
 * @param voltage
 * @param current
 */
void Measurements::addReading(float voltage, float current, float time, int temperature){
    Reading newReading = {voltage, current, time, temperature};
    readings.push_back(newReading);
}

/**
 * @brief Resets the measurement data arrays and capacity values
 */
void Measurements::resetMeasurements(){
    readings.clear();

    mAhCapacity = 0;
    WhCapacity = 0;
}
/*
void Measurements::calculateCapacity(){
    if(numberOfReadings > 2)
        mAhCapacity += (1000*currentReadings[numberOfReadings-1] * (time[numberOfReadings-1] - time[numberOfReadings-2]) / 3600);
}*/

void Measurements::calculateCapacity() {
    int numberOfReadings = readings.size();
    if (numberOfReadings > 1) {
        const Reading& lastReading = readings[numberOfReadings - 1];
        const Reading& secondLastReading = readings[numberOfReadings - 2];

        float deltaTime = (lastReading.time_s - secondLastReading.time_s) / 3600.0; // Time difference in hours
        float averageCurrent = (lastReading.current_A + secondLastReading.current_A) / 2.0; // Average current
        float averageVoltage = (lastReading.voltage_V + secondLastReading.voltage_V) / 2.0; // Average voltage

        // mAh capacity
        mAhCapacity += 1000 * averageCurrent * deltaTime; // Current in mA

        // Wh capacity
        WhCapacity += averageVoltage * averageCurrent * deltaTime; // Energy in Wh
    }
}
