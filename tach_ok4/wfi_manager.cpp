// wifi_manager.cpp
#include <WiFi.h>

#define WIFI_SSID "=]]"
#define WIFI_PASSWORD "thanh8883"

// Hàm kết nối WiFi
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}