#include "RFIDFunctions.h"
#include "FirebaseFunctions.h"
#include "AGV.h"

MFRC522 rfid(SS_PIN, RST_PIN);

String tagIDs[MAX_TAGS];
int tagValues[MAX_TAGS];
int tagCount = 0;

void readRFIDdata() {
  if (Firebase.getJSON(firebaseData, "/RFID_Tags3")) {
    FirebaseJson &json = firebaseData.jsonObject();
    size_t len = json.iteratorBegin();
    String key, value;
    int type;

    for (size_t i = 0; i < len && tagCount < MAX_TAGS; i++) {
      json.iteratorGet(i, type, key, value);
      tagIDs[tagCount] = key;
      tagValues[tagCount] = value.toInt();
      tagCount++;
    }
    json.iteratorEnd();

    Serial.println("Saved Data:");
  } else {
    Serial.println("Failed to get RFID tags from Firebase");
  }
}

void handleRFID() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String tagID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      tagID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      tagID += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.print("Tag ID: ");
    Serial.println(tagID);

    bool tagFound = false;
    for (int i = 0; i < tagCount; i++) {
      if (tagIDs[i] == tagID) {
        int numberTag = tagValues[i];
        Serial.print("NumberTag: ");
        Serial.println(numberTag);

        numberToCoordinate(numberTag, my_agv.x, my_agv.y);

        Serial.print("Coordinates: (x: ");
        Serial.print(my_agv.x);
        Serial.print(", y: ");
        Serial.print(my_agv.y);
        Serial.println(")");

        sendCurrentPosition(numberTag);

        tagFound = true;
        break;
      }
    }

    if (!tagFound) {
      Serial.println("Tag ID not found in memory");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
