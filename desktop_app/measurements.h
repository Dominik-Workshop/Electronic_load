#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

class Measurements{
public:
    Measurements();
    ~Measurements();
    void addReadings(float voltage, float current);
    void resetMeasurements();
    void calculateCapacity();
    float* voltageReadings;
    float* currentReadings;
    float temperature;
    float mAhCapacity;
    float WhCapacity;
    int numberOfReadings; //Number of readings stored
private:
    int arrayCapacity; // Capacity for the readings

};

#endif // MEASUREMENTS_H