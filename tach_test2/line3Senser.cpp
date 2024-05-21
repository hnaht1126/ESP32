#include "line3Senser.h"

/* ------------------------------------------------------------------------- */
/*                                  FUNCTION                                 */
/* ------------------------------------------------------------------------- */
void motor_control(){
  // Read sensor values
  int sensorRight = digitalRead(PIN_OUT1);
  int sensorCenter = digitalRead(PIN_OUT2);
  int sensorLeft = digitalRead(PIN_OUT3);

  // Line following logic
  if (sensorCenter == HIGH) {
    moveForward();
  } else if (sensorLeft == HIGH) {
    turnLeft();
  } else if (sensorRight == HIGH) {
    turnRight();
  } else {
    // All sensors are low, assume it's out of line
    stop();
  }

  delay(100);
}

/* ------------------------- Điều khiển xe dừng lại ------------------------ */

void stop() {
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
  
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

/* ------------------- Điều khiển xe di chuyển tùy chỉnh ------------------- */

void moveForward() {
  analogWrite(PIN_ENA, 150);
  analogWrite(PIN_ENB, 150);
  
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
}

void turnLeft() {
  analogWrite(PIN_ENA, 150);
  analogWrite(PIN_ENB, 150);
  
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
}

void turnRight() {
  analogWrite(PIN_ENA, 150);
  analogWrite(PIN_ENB, 150);
  
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
}

void turnAround() {
  turnRight();
  
  // delay(1000); // Delay for 180 degree turn
}

void setupDriver(){
  // Chan ss
  pinMode(PIN_OUT1, INPUT);
  pinMode(PIN_OUT2, INPUT);
  pinMode(PIN_OUT3, INPUT);
  
  pinMode(IR_SENSOR_PIN, INPUT);

    // Thiết đặt các chân điều khiển Driver
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  // Setup motor speed control pins as output
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  
}