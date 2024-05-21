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

void loop() {
  String nextPosition;

  readDestination();
  nextPosition = readNextPosition();
  Serial.println(nextPosition);

  switch (currentState) {
    case WAIT_FOR_ROAD:
      Serial.println(nextPosition);
      handleRFID();
      Serial.println("State: WAIT_FOR_ROAD");
      
      if (my_agv.x == 0 && my_agv.y == 0) {

        sendNameCar();
      }

      if (nextPosition.indexOf("_") != -1) {
        // if (my_agv.x == 0 && my_agv.y == 0) {
        deleteNameCar();
        currentState = DELIVERING;
        // }
      }
      break;

    case DELIVERING:
      Serial.println("State: DELIVERING");
      navigateToNextPosition(nextPosition);

      if (my_agv.x == target.x && my_agv.y == target.y) {

        // stop();

        servoActivate();
        Serial.println("Activating servo...");
        currentState = RETURN_HOME;
      }
      break;

    case RETURN_HOME:
      Serial.println("State: RETURN_HOME");
      if (nextPosition.indexOf("_") != -1) {
        navigateToNextPosition(nextPosition);
        nextPosition = "";  // Clear nextPosition after navigating
      }
      if (my_agv.x == 0 && my_agv.y == 0) {
        currentState = WAIT_FOR_ROAD;
      }
      break;

    case OBSTACLE_DETECTED:
      Serial.println("State: OBSTACLE_DETECTED");
      
      

  }
}
