/**
 * @file measurements.h
 * @author Dominik Workshop
 * @brief 
 * @version 1.0
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

/**
 * @brief Class for storing measurement data
 */
class Measurements{
public:
    Measurements();
    ~Measurements();
    void addReadings(float voltage, float current, float time);
    void resetMeasurements();
    void calculateCapacity();
    float* voltageReadings;
    float* currentReadings;
    float* time;
    float temperature;
    float mAhCapacity;
    float WhCapacity;
    int mAhNominalCapacity;
    int numberOfReadings; //Number of readings stored
private:
    int arrayCapacity; // Capacity for the readings

};

#endif // MEASUREMENTS_H
