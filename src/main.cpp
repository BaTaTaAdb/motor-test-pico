/****************************************************************
Example code demonstrating the use of the Arduino Library for
the SparkFun MiniMoto board, which uses the TI DRV8830 IC for I2C
low-voltage DC motor control.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

17 Sep 2013- Mike Hord, SparkFun Electronics

Code developed in Arduino 1.0.5, on a Fio classic board.

**Updated for Arduino 1.6.4 5/2015**
****************************************************************/

#include "MiniMotor.h"  // Include the new MiniMoto header
#include <Wire.h>

// Create two MiniMoto instances, with different address settings.
MiniMotor motor0(Wire1, 0x60); // A1 = 1, A0 = clear
MiniMotor motor1(Wire1, 0x65); // A1 = 1, A0 = 1 (default)

#define FAULTn  16     // Pin used for fault detection.

void checkAndReportFault(MiniMotor& motor, int motorNumber);
void delayUntil(unsigned long elapsedTime);

void setup() {
    Wire1.setSDA(6);
    Wire1.setSCL(7);
    Wire1.begin(); // Initialize Wire1 with specific SDA and SCL pins

    motor0.init(); // Initialize the motor0
    motor1.init(); // Initialize the motor1

    Serial.begin(9600);
    Serial.println("Hello, world!");
    pinMode(FAULTn, INPUT);
}

void loop() {
    Serial.println("Forward!");
    motor0.drive(100);
    motor1.drive(100);
    delayUntil(5000);
    Serial.println("Stop!");
    motor0.stop();
    motor1.stop();
    delay(1000);
    Serial.println("Reverse!");
    motor0.drive(-100);
    motor1.drive(-100);
    delayUntil(5000);
    Serial.println("Brake!");
    motor0.brake();
    motor1.brake();
    delay(1000);
}

void delayUntil(unsigned long elapsedTime) {
    unsigned long startTime = millis();
    while (startTime + elapsedTime > millis()) {
        checkAndReportFault(motor0, 0); // Check and report faults for motor0
        checkAndReportFault(motor1, 1); // Check and report faults for motor1
    }
}

void checkAndReportFault(MiniMotor& motor, int motorNumber) {
    byte faultStatus = motor.getFault();

    if (faultStatus & FAULT) {
        Serial.print("Motor ");
        Serial.print(motorNumber);
        Serial.print(" fault: ");

        if (faultStatus & OCP) Serial.println("Chip overcurrent!");
        if (faultStatus & ILIMIT) Serial.println("Load current limit!");
        if (faultStatus & UVLO) Serial.println("Undervoltage!");
        if (faultStatus & OTS) Serial.println("Over temp!");
    }
}