// firebase_manager.h
#ifndef FIREBASE_MANAGER_H
#define FIREBASE_MANAGER_H

#include <FirebaseESP32.h>

void initializeFirebase();
String readNextPosition();
void sendCurrentPosition(int current_x, int current_y);
void removeProcessedNumberFromFirebase();

#endif