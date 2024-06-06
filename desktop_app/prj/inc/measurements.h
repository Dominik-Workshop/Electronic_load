/**
 * @file measurements.h
 * @author Dominik Workshop
 * @brief Declaration of the Measurements class and
 *        the Reading struct for storing and managing measurement data.
 *
 * @version 1.1
 * @date 2024-05-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <vector>
#include <list>

/**
 * @brief Struct for storing measured voltage, current,
 * tepmerature of the load's heatsink and time of the measurement
 *
 */
struct Reading{
    float voltage_V;
    float current_A;
    float time_s;
    int temperature_C;
};

/**
 * @brief Class for storing measurement data
 *
 */
class Measurements{
public:
    Measurements();
    ~Measurements();
    void addReading(float voltage, float current, float time, int temperature);
    void resetMeasurements();
    void calculateCapacity();

    std::list<Reading> readings;

    float capacity_mAh;
    float capacity_Wh;
    int nominalCapacity_mAh;
private:
};

#endif // MEASUREMENTS_H
