#include <MemoryFree.h>

// On Arduino Leonardo with ATmega32U4:
//
// Reported free memory with str commented out:
// 2373 bytes
//
// Reported free memory with str and Serial.println(str) uncommented:
// 2359 bytes
//
// Difference: 14 bytes (13 ASCII chars + null terminator)

// 14-bytes string
//char str[] = "Hello, world!";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {
//  Serial.println(str);

  Serial.print("freeMemory()=");
  Serial.println(freeMemory());

  delay(1000);
}
