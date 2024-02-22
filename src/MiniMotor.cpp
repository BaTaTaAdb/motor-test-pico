#include <Arduino.h>
#include <Wire.h>

class MiniMoto {
public:
    MiniMoto(byte addr); // Constructor

    byte getFault();     // Get the fault status of the DRV8830 chip
    void drive(int speed); // Control motor speed and direction
    void stop();         // Stop the motor with high impedance
    void brake();        // Brake the motor

private:
    byte _addr; // I2C address of the MiniMoto board

    // Original low-level I2C methods (commented out)
    // void I2CReadBytes(byte addr, byte *buffer, byte len);
    // void I2CWriteBytes(byte addr, byte *buffer, byte len);

    // New I2C methods using Wire library
    void I2CReadBytes_Wire(byte addr, byte* buffer, byte len);
    void I2CWriteBytes_Wire(byte addr, byte* buffer, byte len);
};

MiniMoto::MiniMoto(byte addr) {
    _addr = addr;
    Wire.begin(); // Initialize the Wire library
}

void MiniMoto::I2CReadBytes_Wire(byte reg, byte* buffer, byte len) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom((int)_addr, (int)len);
    for (byte i = 0; i < len; i++) {
        if (Wire.available()) buffer[i] = Wire.read();
    }
}

void MiniMoto::I2CWriteBytes_Wire(byte reg, byte* buffer, byte len) {
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    for (byte i = 0; i < len; i++) {
        Wire.write(buffer[i]);
    }
    Wire.endTransmission();
}

// Return the fault status of the DRV8830 chip. Also clears any existing faults.
byte MiniMoto::getFault() {
    byte buffer[1] = { 0 };
    byte clearFault = 0x80;
    I2CReadBytes_Wire(0x01, buffer, 1);
    I2CWriteBytes_Wire(0x01, &clearFault, 1);
    return buffer[0];
}

// Send the drive command over I2C to the DRV8830 chip.
void MiniMoto::drive(int speed) {
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
void MiniMoto::stop() {
    byte regValue = 0; // See above for bit 1:0 explanation.
    I2CWriteBytes_Wire(0x00, &regValue, 1);
}

// Stop the motor by providing a heavy load on it.
void MiniMoto::brake() {
    byte regValue = 0x03; // See above for bit 1:0 explanation.
    I2CWriteBytes_Wire(0x00, &regValue, 1);
}
