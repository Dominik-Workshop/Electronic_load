// example useage
#include <Arduino.h>
#include <MCP79410_Timer.h>

const byte MCP79410_ADDRESS = 0x6f;

MCP79410_Timer timer = MCP79410_Timer(MCP79410_ADDRESS);


void testTimer(int secondsToTest, int delayMs){
  int i = 0;
  while (i < secondsToTest){
    delay(delayMs);
    uint32_t seconds = timer.getTotalSeconds();
    String strTime = timer.getTime();
    Serial.println(seconds);
    Serial.println(strTime);
    i++;
  }
}


void setup()
{
   Serial.begin(9600);
}

void loop(){
    Serial.println("Timer Test");
    Serial.println("==========");
    // reset the timer
    timer.reset();

    Serial.println("-----------");
    Serial.println("Timer Reset");
    Serial.println("-----------");

    // start the timer
    timer.start();

    Serial.println("-------------");
    Serial.println("Timer Started");
    Serial.println("-------------");

    // test the timer output;
    testTimer(5,1000);

    // stop the timer
    timer.stop();

    Serial.println("-------------");
    Serial.println("Timer Stopped");
    Serial.println("-------------");

    // test the timer output;
    testTimer(2,1000);

    // start the timer
    timer.start();

    Serial.println("-------------");
    Serial.println("Timer Started");
    Serial.println("-------------");

    // test the timer output;
    testTimer(5,1000);

    Serial.println("-----------");
    Serial.println("Timer Reset");
    Serial.println("-----------");

    // reset the timer
    // reset automatically stops the timer
    timer.reset();

    // test the timer output;
    testTimer(2,1000);

    // start the timer
    timer.start();

    Serial.println("-------------");
    Serial.println("Timer Started");
    Serial.println("-------------");

    // test the timer output;
    testTimer(5,1000);

    // reset the timer
    // reset automatically stops the timer
    timer.reset();

    Serial.println("==========");
    Serial.println("Test Ended");
    Serial.println("==========");

    delay(5000);
}
