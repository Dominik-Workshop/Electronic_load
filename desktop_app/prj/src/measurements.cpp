/**
 * @file measurements.cpp
 * @author Dominik Workshop
 * @brief Implementation of the Measurements class methods
 *        for storing and managing measurement data.
 *
 * @version 1.1
 * @date 2024-05-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>

#include "measurements.h"

Measurements::Measurements(){
    capacity_mAh = 0;
    capacity_Wh = 0;
    nominalCapacity_mAh = 0;
}

Measurements::~Measurements(){
}

/**
 * @brief Adds a new reading to the readings vector
 *
 * @param voltage Voltage value in volts
 * @param current Current value in amperes
 * @param time Time value in seconds
 * @param temperature Temperature value in degrees Celsius
 */
void Measurements::addReading(float voltage, float current, float time, int temperature){
    Reading newReading = {voltage, current, time, temperature};
    readings.push_back(newReading);
}

/**
 * @brief Resets the measurement data vector and capacity values
 */
void Measurements::resetMeasurements(){
    readings.clear();

    capacity_mAh = 0;
    capacity_Wh = 0;
}

/**
 * @brief Calculates the battery capacity based on the readings.
 */
void Measurements::calculateCapacity() {
    if (readings.size() < 2) {
        return; // Not enough data to calculate capacity
    }

    int numberOfReadings = readings.size();
    const Reading& lastReading = readings[numberOfReadings - 1];
    const Reading& secondLastReading = readings[numberOfReadings - 2];

    float deltaTime_h = (lastReading.time_s - secondLastReading.time_s) / 3600.0;
    float averageCurrent_A = (lastReading.current_A + secondLastReading.current_A) / 2.0;
    float averageVoltage_V = (lastReading.voltage_V + secondLastReading.voltage_V) / 2.0;

    capacity_mAh += 1000 * averageCurrent_A * deltaTime_h;
    capacity_Wh += averageVoltage_V * averageCurrent_A * deltaTime_h;
}
