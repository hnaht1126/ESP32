#include "ServoFunctions.h"

Servo servoMotor; 

void servoActivate(){
  servoMotor.write(90);
  delay(2000);
  servoMotor.write(0);
}