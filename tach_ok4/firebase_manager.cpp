// firebase_manager.cpp
#include "firebase_manager.h"

#define FIREBASE_HOST "https://datn-2024-5610e-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "ueckKrNn2oImXTsmd6NFs37bk8QHcV2yLU4ImENr"

FirebaseData firebaseData;

void initializeFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Firebase connected");
}

// Hàm đọc chuỗi tọa độ từ Firebase
String readNextPosition() {
  if (Firebase.get(firebaseData, "/Cars/Car-A-1/Road")) {
    return firebaseData.stringData();
  } else {
    Serial.println("Failed to read next position from Firebase");
    return "";
  }
}

// Hàm gửi tọa độ hiện tại của xe lên Firebase
void sendCurrentPosition(int current_x, int current_y) {
  // Tạo một đường dẫn Firebase cho tọa độ hiện tại của xe
  String path = "/Cars/Car-A-1/CurrentPosition";

  // Tạo một chuỗi chứa tọa độ hiện tại của xe
  String currentPosition = String(current_x) + "," + String(current_y);

  // Gửi dữ liệu lên Firebase
  if (!Firebase.setString(firebaseData, path.c_str(), currentPosition)) {
    Serial.println("Failed to send current position to Firebase");
  }
}

// Hàm loại bỏ số đã xử lý khỏi chuỗi trên Firebase
void removeProcessedNumberFromFirebase() {
  String path = "/Cars/Car-A-1/Road";

  // Đọc chuỗi số từ Firebase
  String currentDestination = readNextPosition();

  // Tìm vị trí của dấu gạch dưới đầu tiên trong chuỗi
  int underscoreIndex = currentDestination.indexOf("_");
  if (underscoreIndex != -1) {
    // Nếu tìm thấy dấu gạch dưới, loại bỏ số đã xử lý từ chuỗi
    currentDestination.remove(0, underscoreIndex + 1);

    // Gửi chuỗi đã chỉnh sửa trở lại Firebase để cập nhật chuỗi destination mới
    if (!Firebase.set(firebaseData, path, currentDestination)) {
      Serial.println("Failed to update destination on Firebase");
      // Xử lý khi không thể cập nhật Firebase
    }
  } else {
    // Nếu không tìm thấy dấu gạch dưới, có thể hiểu là đã xử lý hết chuỗi
    // Trong trường hợp này, bạn có thể xử lý một cách phù hợp hoặc không cần gửi yêu cầu lên Firebase
    // Ví dụ: gửi một tín hiệu hoặc thông báo rằng tất cả các điểm đã được xử lý
  }
}