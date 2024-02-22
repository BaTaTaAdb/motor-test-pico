#ifndef MiniMotor_h
#define MiniMotor_h

#include <Arduino.h>
#include <Wire.h>

#define FAULT 0x01
#define ILIMIT 0x10
#define OTS 0x08
#define UVLO 0x04
#define OCP 0x02

class MiniMoto {
public:
    MiniMoto(byte addr); // Constructor

    byte getFault();     // Get the fault status of the DRV8830 chip
    void drive(int speed); // Control motor speed and direction
    void stop();         // Stop the motor with high impedance
    void brake();        // Brake the motor

private:
    byte _addr; // I2C address of the MiniMoto board

    // I2C communication methods using Wire library
    void I2CReadBytes_Wire(byte reg, byte* buffer, byte len);
    void I2CWriteBytes_Wire(byte reg, byte* buffer, byte len);
};

#endif
