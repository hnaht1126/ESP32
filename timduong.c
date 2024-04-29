#include <WiFi.h>
#include "FirebaseESP32.h"

#define FIREBASE_HOST "https://datn-2024-5610e-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "ueckKrNn2oImXTsmd6NFs37bk8QHcV2yLU4ImENr"

#define WIFI_SSID "=]]"
#define WIFI_PASSWORD "thanh8883"

FirebaseData firebaseData;

// Định nghĩa các hướng
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define BACKWARD 4

// Tên các hướng
const char *direction_names[] = { "UP", "DOWN", "LEFT", "RIGHT" };

// Struct đại diện cho xe AGV
typedef struct {
  int direction;  // Hướng của xe AGV
  int x;          // Tọa độ x
  int y;          // Tọa độ y
} AGV;

// Struct đại diện cho tọa độ
typedef struct {
  int x;  // Tọa độ x
  int y;  // Tọa độ y
} Coordinate;

// Hàm thay đổi hướng của xe AGV
void change_direction(AGV *agv, int new_direction) {
  if (agv->direction != new_direction) {
    int turn_direction;
    // Xác định hướng cần quay
    if ((agv->direction == UP && new_direction == RIGHT) || 
        (agv->direction == RIGHT && new_direction == DOWN) || 
        (agv->direction == DOWN && new_direction == LEFT) || 
        (agv->direction == LEFT && new_direction == UP)) {
      turn_direction = RIGHT;
    } else if ((agv->direction == UP && new_direction == LEFT) || 
               (agv->direction == RIGHT && new_direction == UP) || 
               (agv->direction == DOWN && new_direction == RIGHT) || 
               (agv->direction == LEFT && new_direction == DOWN)) {
      turn_direction = LEFT;
    } else if ((agv->direction == UP && new_direction == DOWN) || 
               (agv->direction == RIGHT && new_direction == LEFT) || 
               (agv->direction == DOWN && new_direction == UP) || 
               (agv->direction == LEFT && new_direction == RIGHT)) {
      turn_direction = BACKWARD;
    } else {
      turn_direction = -1; // Không cần quay
    }

    // Thực hiện lệnh quay
    if (turn_direction == RIGHT) {
      // turn_right();
      Serial.println("Turn right");
    } else if (turn_direction == LEFT) {
      // turn_left();
      Serial.println("Turn left");
    } else if (turn_direction == BACKWARD) {
      // turn_backward();
      Serial.println("Go backward");
    }

    // Thay đổi hướng
    agv->direction = new_direction;
  } else {
    Serial.println("No turn needed");
  }
}

// Hàm di chuyển thẳng của xe AGV
void move_forward(AGV *agv) {

// Di chuyển theo hướng hiện tại
  switch (agv->direction) {
    case UP:
      agv->y--;
      break;
    case DOWN:
      agv->y++;
      break;
    case LEFT:
      agv->x--;
      break;
    case RIGHT:
      agv->x++;
      break;
  }

  // hàm di chuyển
  // move_forward();

   // Gửi tọa độ mới lên Firebase
  sendCurrentPosition(agv->x, agv->y);

  Serial.print("Move forward to: (");
  Serial.print(agv->x);
  Serial.print(", ");
  Serial.print(agv->y);
  Serial.println(")");
}

// Hàm xác định hướng mới dựa trên tọa độ của điểm tiếp theo và tọa độ hiện tại của xe AGV
int determine_new_direction(int current_x, int current_y, int next_x, int next_y) {
  if (next_x > current_x) {
    return RIGHT;
  } else if (next_x < current_x) {
    return LEFT;
  } else if (next_y > current_y) {
    return DOWN;
  } else if (next_y < current_y) {
    return UP;
  } else {
    // Trường hợp đặc biệt: tọa độ hiện tại và tọa độ tiếp theo giống nhau
    return -1;  // Trả về giá trị không hợp lệ để xác định rằng không cần thay đổi hướng
  }
}

// Hàm thay đổi hướng dựa trên lệnh di chuyển tiếp theo và hướng hiện tại của xe AGV
void adjust_direction(AGV *agv, int next_x, int next_y) {
  // Xác định hướng mới dựa trên tọa độ của điểm tiếp theo và tọa độ hiện tại của xe AGV
  int new_direction = determine_new_direction(agv->x, agv->y, next_x, next_y);

  // Thay đổi hướng nếu cần thiết
  if (new_direction != -1) {
    // Thực hiện lệnh quay
    change_direction(agv, new_direction);
  }
}

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

// Convert number to coordinate
void numberToCoordinate(int number, int &x, int &y) {
  x = number % 10;
  y = number / 10;
}

 // Tạo một đối tượng AGV
  AGV my_agv = { RIGHT, 0, 0 };


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

void setup() {
  Serial.begin(9600);
  connectWiFi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Firebase connected");
}

void loop() {
  // Đọc chuỗi tọa độ từ Firebase
  String nextPosition = readNextPosition();
  if (nextPosition != "") {
    navigateToNextPosition(nextPosition);
  }
}
