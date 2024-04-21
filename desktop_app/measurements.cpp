

#include <stdlib.h>

#include "measurements.h"

Measurements::Measurements(){
    voltageReadings = nullptr;
    voltageCount = 0;
    numberOfReadings = 0;
}

void Measurements::addVoltage(float value) {
    /*if (voltageCount >= numberOfReadings) {
        // If the array is full, increase capacity
        int newCapacity = (numberOfReadings == 0) ? 1 : numberOfReadings * 2;
        float* newArray = (float*)realloc(voltageReadings, newCapacity * sizeof(float));
        if (newArray == nullptr) {
            // Handle memory allocation failure
            // For example, you could throw an exception or return an error code
            // Here, let's just print an error message and return
            return;
        }
        // Update the array and capacity
        voltageReadings = newArray;
        numberOfReadings = newCapacity;
    }
    // Add the new voltage value*/
    ++numberOfReadings;
    voltageReadings = (float*)realloc(voltageReadings, (numberOfReadings * sizeof(float)));
    voltageReadings[numberOfReadings - 1] = value;

}
