/* #include <WiFi.h>
#include "FirebaseESP32.h"

#include <SPI.h>
#include <MFRC522.h> */

#include "AGV.h"
#include "FirebaseFunctions.h"
#include "RFIDFunctions.h"
#include "ServoFunctions.h"
// #include "line3Senser.h"



void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  connectWiFi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Firebase connected");

  readRFIDdata();

  servoMotor.setPeriodHertz(50);
  servoMotor.attach(SERVO_PIN);

  setupDriver();
}

String nextPosition;
void loop() {

  Serial.println("loop");

  switch (currentState) {
      // case CHECK_WAIT:
      //   Serial.println("State: CHECK_WAIT");

    case WAIT_FOR_ROAD:
      handleRFID();
      Serial.println("State: WAIT_FOR_ROAD");
      readDestination();
      nextPosition = readNextPosition();
      Serial.println(nextPosition);
      if (my_agv.x == 24 && my_agv.y == 1) {

        sendNameAwaitCar();
      }

      if (nextPosition.indexOf("_") != -1) {
        if (my_agv.x == 24 && my_agv.y == 1) {
          deleteNameAwaitCar();
          currentState = DELIVERING;
        }
      }
      break;

    case DELIVERING:
      Serial.println("State: DELIVERING");

      /*  if (digitalRead(IR_SENSOR_PIN) == LOW) {
        sendObstacleDetected();
        // delay(1000);
        currentState = OBSTACLE_DETECTED;
        break;
      } */

      navigateToNextPosition(nextPosition);
      if (my_agv.x == target.x && my_agv.y == target.y) {
        stop();
        servoActivate();
        Serial.println("Activating servo...");
        currentState = RETURN_HOME;
      }
      break;

    case RETURN_HOME:
      Serial.println("State: RETURN_HOME");

      /*   if (digitalRead(IR_SENSOR_PIN) == LOW) {
        sendObstacleDetected();
        currentState = OBSTACLE_DETECTED;
        break;
      } */

      if (nextPosition.indexOf("_") != -1) {
        navigateToNextPosition(nextPosition);
        nextPosition = "";  // Clear nextPosition after navigating
      }
      if (my_agv.x == 24 && my_agv.y == 1) {
        currentState = WAIT_FOR_ROAD;
      }
      break;


      /* case OBSTACLE_DETECTED:
      Serial.println("State: OBSTACLE_DETECTED");

      if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
        handleRFID();
        stop();
      } else {
        while (digitalRead(PIN_OUT3) == HIGH) {
          turnAround();
          // delay(500);
        }
        my_agv.direction = (my_agv.direction + 2) % 4;  // Update direction after 180 turn
        navigateUntilRFID();
      }
        deleteObstacleDetected();

      if (target.x == 0 && target.y == 0) {
        currentState = RETURN_HOME;
      } else {
        currentState = DELIVERING;
      }
      break; */
  }
}
