#ifndef MiniMotor_h
#define MiniMotor_h

#include <Arduino.h>
#include <Wire.h>

#define FAULT 0x01
#define ILIMIT 0x10
#define OTS 0x08
#define UVLO 0x04
#define OCP 0x02

class MiniMotor {
public:
    MiniMotor(TwoWire& wire, byte addr);
    void init(); // New method to initialize the I2C bus

    byte getFault();
    void drive(int speed);
    void stop();
    void brake();

private:
    TwoWire& _wire;
    byte _addr;
    void I2CReadBytes_Wire(byte reg, byte* buffer, byte len);
    void I2CWriteBytes_Wire(byte reg, byte* buffer, byte len);
};

#endif