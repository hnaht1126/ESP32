#ifndef SERVO_FUNCTION_H
#define SERVO_FUNCTION_H

#include <ESP32Servo.h>

#define SERVO_PIN 22  // !D22

extern Servo servoMotor; 

void servoActivate();

#endif