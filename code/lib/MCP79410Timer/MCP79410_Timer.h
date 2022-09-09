#ifndef MCP79410_Timer_h
#define MCP79410_Timer_h

#include <Arduino.h>
#include <Wire.h>

class MCP79410_Timer {
  public:
    MCP79410_Timer(byte rtcAddress);
    MCP79410_Timer(void);
    void start();
    void stop();
    void reset();
    int status();
    uint8_t hours();
    uint8_t minutes();
    uint8_t seconds();
    String getTime();
    uint32_t getTotalSeconds();

  private:
    // private methods
    unsigned char _readRtcByte(const unsigned char adr);
    void _writeRtcByte(const unsigned char adr, const unsigned char data);
    uint8_t _makeDec(uint8_t num);
    uint8_t _makeHex(uint8_t num);
    unsigned char _getRtcData(const unsigned char adr, const unsigned char validbits);
    String _format2digit(const unsigned char data);
    // private variables
    byte _rtcAddress;
    int _rtcState;

};

#endif
