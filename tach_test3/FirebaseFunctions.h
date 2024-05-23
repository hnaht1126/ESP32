#ifndef FIREBASE_FUNCTIONS_H
#define FIREBASE_FUNCTIONS_H

#include <WiFi.h>
#include "FirebaseESP32.h"
#include "AGV.h"

// Firebase credentials
#define FIREBASE_HOST "https://datn-2024-5610e-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "ueckKrNn2oImXTsmd6NFs37bk8QHcV2yLU4ImENr"

#define WIFI_SSID "=]]"
#define WIFI_PASSWORD "thanh8883"

extern FirebaseData firebaseData;

void connectWiFi();
String readNextPosition();
void readDestination();
void sendCurrentPosition(int number);
void removeProcessedNumberFromFirebase();
void numberToCoordinate(int number, int &x, int &y);

void sendNameAwaitCar();
void deleteNameAwaitCar();
/* 
void sendObstacleDetected();
void deleteObstacleDetected(); */

#endif
