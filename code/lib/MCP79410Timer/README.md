# MCP79410_Timer
Timer library for the MCP79410 RTC Clock Chip, Initially for use with Scullcom Hobby Electronics DC Load.

This library provides functions required to implement a timer, not a full real-time clock.

## Usage Example
Below is an example of how the methods can be called in your Arduino Sketch:
```c++
#include <Arduino.h>
#include <MCP79410_Timer.h>

// define the address of the MCP79410
const byte MCP79410_ADDRESS = 0x6f;

// instantiate a instance of the timer to use
MCP79410_Timer timer = MCP79410_Timer(MCP79410_ADDRESS);

void setup()
{
   Serial.begin(9600);
}

void loop(){    
    // reset the timer
    timer.reset();

    // start the timer
    timer.start();

    // get the timer time in seconds
    uint32_t seconds = timer.getTotalSeconds();

    // get the timer time in HH:MM:SS
    String strTime = timer.getTime();

    // print on the console
    Serial.println(seconds);
    Serial.println(strTime);

    // stop the timer
    timer.stop();

    // reset the timer
    timer.reset();
}
```

A more detailed example showing the timer in use can be found in the examples directory.

## Public methods

### void start()
This starts the timer running

### void stop()
This stops the timer running. The current time is maintained

### void reset()
This stops the timer and resets the held time back to 0

### uint_32 getTotalSeconds()
This returns the totalSeconds that have elapsed since the timer was Started

### String getTime()
This returns the time that has elapsed since the timer was started in hours minutes and seconds (format: HH:MM:SS)

## Credit
Borrowed extensively from Ian Chilton: https://github.com/ichilton/mcp7941x_arduino.
