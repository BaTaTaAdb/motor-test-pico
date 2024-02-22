#include <Arduino.h>
#include <Wire.h>
#include "MiniMotor.h"

MiniMotor::MiniMotor(TwoWire& wire, byte addr) : _wire(wire), _addr(addr) {
    // Constructor body (if necessary) 
};

void MiniMotor::init() {
    _wire.begin(); // Initialize the I2C bus here
};

// I2C Read Bytes Method
void MiniMotor::I2CReadBytes_Wire(byte reg, byte* buffer, byte len) {
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.endTransmission();

    _wire.requestFrom((int)_addr, (int)len);
    for (byte i = 0; i < len; i++) {
        if (_wire.available()) buffer[i] = _wire.read();
    }
}

// I2C Write Bytes Method
void MiniMotor::I2CWriteBytes_Wire(byte reg, byte* buffer, byte len) {
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    for (byte i = 0; i < len; i++) {
        _wire.write(buffer[i]);
    }
    _wire.endTransmission();
}

// Return the fault status of the DRV8830 chip. Also clears any existing faults.
byte MiniMotor::getFault() {
    byte buffer[1] = { 0 };
    byte clearFault = 0x80;
    I2CReadBytes_Wire(0x01, buffer, 1);
    I2CWriteBytes_Wire(0x01, &clearFault, 1);
    return buffer[0];
}

// Send the drive command over I2C to the DRV8830 chip.
void MiniMotor::drive(int speed) {
    byte regValue = 0x80; // Clear the fault status.
    I2CWriteBytes_Wire(0x01, &regValue, 1); // Clear the fault status.

    regValue = (byte)abs(speed); // Find the byte-ish abs value of the input
    if (regValue > 63) regValue = 63; // Cap the value at 63.
    regValue = regValue << 2; // Left shift to make room for bits 1:0

    if (speed < 0) regValue |= 0x01; // Set bits 1:0 based on sign of input.
    else regValue |= 0x02;

    I2CWriteBytes_Wire(0x00, &regValue, 1);
}

// Coast to a stop by hi-z'ing the drivers.
void MiniMotor::stop() {
    byte regValue = 0; // See above for bit 1:0 explanation.
    I2CWriteBytes_Wire(0x00, &regValue, 1);
}

// Stop the motor by providing a heavy load on it.
void MiniMotor::brake() {
    byte regValue = 0x03; // See above for bit 1:0 explanation.
    I2CWriteBytes_Wire(0x00, &regValue, 1);
}