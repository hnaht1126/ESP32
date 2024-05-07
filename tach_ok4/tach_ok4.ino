#include <WiFi.h>
#include "FirebaseESP32.h"
#include "wifi_manager.h"
#include "firebase_manager.h"
#include "agv_controller.h"


  // Initialize AGV
  AGV my_agv;


void setup() {
  Serial.begin(9600);
  connectWiFi();
  initializeFirebase();

  initializeAGV(&my_agv);
}


void loop() {
  // Read next position from Firebase
  String nextPosition = readNextPosition();
  if (nextPosition != "") {
    navigateToNextPosition(nextPosition);
  }
  
  // Other loop tasks...
}

void navigateToNextPosition(String nextPosition) {
 
  // Lặp qua chuỗi tọa độ để tìm vị trí dấu gạch dưới và phân tách thành các phần tử riêng lẻ
  while (nextPosition.indexOf("_") != -1) {
    // Tìm vị trí của dấu gạch dưới trong chuỗi
    int underscoreIndex = nextPosition.indexOf("_");

    // Tách chuỗi để lấy phần tử đầu tiên
    String token = nextPosition.substring(0, underscoreIndex);

    // Chuyển đổi token thành số nguyên và tính toán tọa độ x và y
    int number = token.toInt();
    int target_x, target_y;
    numberToCoordinate(number, target_x, target_y);

    // Điều chỉnh hướng dựa trên tọa độ của điểm tiếp theo
    adjust_direction(&my_agv, target_x, target_y);

    // Di chuyển đến tọa độ mới
    move_forward(&my_agv);

    // Loại bỏ phần tử đã xử lý khỏi chuỗi nextPosition
    nextPosition = nextPosition.substring(underscoreIndex + 1);

    removeProcessedNumberFromFirebase();

  }
}
