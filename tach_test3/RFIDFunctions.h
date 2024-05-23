#ifndef RFID_FUNCTIONS_H
#define RFID_FUNCTIONS_H

#include <SPI.h>
#include <MFRC522.h>

// RFID pins
#define SS_PIN 15
#define RST_PIN 4

extern MFRC522 rfid;

const int MAX_TAGS = 100;
extern String tagIDs[MAX_TAGS];
extern int tagValues[MAX_TAGS];
extern int tagCount;

void readRFIDdata();
void handleRFID();

#endif
