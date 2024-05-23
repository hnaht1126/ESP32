#include "FirebaseFunctions.h"

FirebaseData firebaseData;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

String readNextPosition() {
  if (Firebase.get(firebaseData, "/Cars/Car-A-1/Road")) {
    return firebaseData.stringData();
  }
}

void readDestination() {
  if (Firebase.get(firebaseData, "/Cars/Car-A-1/Destination")) {
    int destination = firebaseData.intData();
    numberToCoordinate(destination, target.x, target.y);
  }
}

void sendCurrentPosition(int numberTag) {
  String path = "/Cars/Car-A-1/CurrentPosition";
  Firebase.setInt(firebaseData, path, numberTag);
}

void removeProcessedNumberFromFirebase() {
  String path = "/Cars/Car-A-1/Road";
  String currentDestination = readNextPosition();
  int underscoreIndex = currentDestination.indexOf("_");
  if (underscoreIndex != -1) {
    currentDestination.remove(0, underscoreIndex + 1);
    if (!Firebase.set(firebaseData, path, currentDestination)) {
      Serial.println("Failed to update destination on Firebase");
    }
  } else {
    Serial.println("All points processed");
  }
}

void numberToCoordinate(int number, int &x, int &y) {
    x = number % 26;
    y = number / 26;
}

void sendNameAwaitCar(){
  String path = "/Cars/AwaitCar";
  String nameCar = "Car1";
  Firebase.setString(firebaseData, path, nameCar);

}

void deleteNameAwaitCar(){
  String path = "/Cars/AwaitCar";
  Firebase.setString(firebaseData, path, " ");
}


/* void deleteNameCar() {
  String path = "/Cars/AwaitCar";
  Firebase.deleteNode(firebaseData, path);
} */

/* void sendObstacleDetected() {
  if (Firebase.setString(firebaseData, "/AGV_Obstacle", "Obstacle detected")) {
    Serial.println("Obstacle detected signal sent to Firebase");
 } 
}

void deleteObstacleDetected(){
  Firebase.setString(firebaseData, "/AGV_Obstacle", " ");
} */

