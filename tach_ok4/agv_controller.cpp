// agv_controller.cpp
#include "agv_controller.h"
#include "firebase_manager.h"

// Định nghĩa các hướng
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define BACKWARD 4

const char *direction_names[] = { "UP", "DOWN", "LEFT", "RIGHT" };

void initializeAGV(AGV *agv) {
  // Khởi tạo hướng ban đầu của AGV
  agv->direction = RIGHT;

  // Khởi tạo tọa độ ban đầu của AGV
  agv->x = 0;
  agv->y = 0;
}

// Hàm thay đổi hướng của xe AGV
void change_direction(AGV *agv, int new_direction) {
  if (agv->direction != new_direction) {
    int turn_direction;
    // Xác định hướng cần quay
    if ((agv->direction == UP && new_direction == RIGHT) || (agv->direction == RIGHT && new_direction == DOWN) || (agv->direction == DOWN && new_direction == LEFT) || (agv->direction == LEFT && new_direction == UP)) {
      turn_direction = RIGHT;
    } else if ((agv->direction == UP && new_direction == LEFT) || (agv->direction == RIGHT && new_direction == UP) || (agv->direction == DOWN && new_direction == RIGHT) || (agv->direction == LEFT && new_direction == DOWN)) {
      turn_direction = LEFT;
    } else if ((agv->direction == UP && new_direction == DOWN) || (agv->direction == RIGHT && new_direction == LEFT) || (agv->direction == DOWN && new_direction == UP) || (agv->direction == LEFT && new_direction == RIGHT)) {
      turn_direction = BACKWARD;
    } else {
      turn_direction = -1;  // Không cần quay
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


// đổi số sang tọa độ 
void numberToCoordinate(int number, int &x, int &y) {
  x = number % 10;
  y = number / 10;
}

