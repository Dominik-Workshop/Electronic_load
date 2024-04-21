#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

class Measurements{
public:
    Measurements();
    void addVoltage(float value);
    void resetMeasurement();
    void calculateCapacity();
    float* voltageReadings;
    float current;
    float temperature;
    float mAhCapacity;
    float WhCapacity;
    int voltageCount; // Number of voltage readings stored
    int numberOfReadings; // Capacity of the voltage array

private:


};

#endif // MEASUREMENTS_H
