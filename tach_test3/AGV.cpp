#include "AGV.h"

AGVState currentState = WAIT_FOR_ROAD;
Coordinate next = { 0, 0 };
Coordinate target = { 0, 0 };
AGV my_agv = { LEFT, -1, -1 };

TaskHandle_t MoveTaskHandle = NULL;

void move_forward_task(void *parameter) {
  while (true) {
    motor_control();
    Serial.println("Moving forward");
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Adjust delay as needed
  }
}

void start_movement_task() {
  if (MoveTaskHandle == NULL) {
    xTaskCreatePinnedToCore(
      move_forward_task,  // Function that should be called
      "MoveForwardTask",  // Name of the task (for debugging)
      10000,              // Stack size (bytes)
      NULL,               // Parameter to pass
      1,                  // Task priority
      &MoveTaskHandle,    // Task handle
      0                   // Core to run the task on (0 or 1)
    );
  }
}

void stop_movement_task() {
  if (MoveTaskHandle != NULL) {
    Serial.println("ngat");
    vTaskDelete(MoveTaskHandle);
    MoveTaskHandle = NULL;
  }
}

void change_direction(AGV *agv, int new_direction) {
  if (agv->direction != new_direction) {
    int turn_direction;
    if ((agv->direction == UP && new_direction == RIGHT) || (agv->direction == RIGHT && new_direction == DOWN) || (agv->direction == DOWN && new_direction == LEFT) || (agv->direction == LEFT && new_direction == UP)) {
      turn_direction = RIGHT;
    } else if ((agv->direction == UP && new_direction == LEFT) || (agv->direction == RIGHT && new_direction == UP) || (agv->direction == DOWN && new_direction == RIGHT) || (agv->direction == LEFT && new_direction == DOWN)) {
      turn_direction = LEFT;
    } else if ((agv->direction == UP && new_direction == DOWN) || (agv->direction == RIGHT && new_direction == LEFT) || (agv->direction == DOWN && new_direction == UP) || (agv->direction == LEFT && new_direction == RIGHT)) {
      turn_direction = BACKWARD;
    } else {
      turn_direction = -1;  // No turn needed
    }

    if (turn_direction == RIGHT) {
      while (true) {
        turnRight();
        Serial.println("Turn right");
        if (digitalRead(PIN_OUT3) == HIGH) {
          break;
        }
      }

    } else if (turn_direction == LEFT) {
      while (true) {
        turnLeft();
        Serial.println("Turn left");
        if (digitalRead(PIN_OUT3) == HIGH) {
          break;
        }
      }


    } else if (turn_direction == BACKWARD) {
      while (true) {
        turnAround();
        Serial.println("Go backward");
        if (digitalRead(PIN_OUT3) == HIGH) {
          break;
        }
      }
    }

    agv->direction = new_direction;
  } else {
    Serial.println("No turn needed");
  }
}

int determine_new_direction(AGV *agv, Coordinate next) {
  if (next.x > agv->x) {
    return RIGHT;
  } else if (next.x < agv->x) {
    return LEFT;
  } else if (next.y > agv->y) {
    return UP;
  } else if (next.y < agv->y) {
    return DOWN;
  } else {
    return -1;  // No direction change needed
  }
}

void adjust_direction(AGV *agv, Coordinate next) {
  int new_direction = determine_new_direction(agv, next);
  if (new_direction != -1) {
    change_direction(agv, new_direction);
  }
}

/* void move_forward() {
  motor_control();
  Serial.println("Moving forward");
} */


void navigateToNextPosition(String nextPosition) {
  if (nextPosition.indexOf("_") == -1) {
    return;
  }

  int underscoreIndex = nextPosition.indexOf("_");
  String token = nextPosition.substring(0, underscoreIndex);

  int numberNext = token.toInt();

  numberToCoordinate(numberNext, next.x, next.y);

  Serial.print("next to: ");
  Serial.print(next.x);
  Serial.print(", ");
  Serial.println(next.y);

  adjust_direction(&my_agv, next);

  start_movement_task();
  do{
      handleRFID();
  }while(my_agv.x != next.x || my_agv.y != next.y);

  stop_movement_task();

  if (my_agv.x == next.x && my_agv.y == next.y) {
    nextPosition = nextPosition.substring(underscoreIndex + 1);
    Serial.println(nextPosition);
    removeProcessedNumberFromFirebase();
  }
}



/* void navigateUntilRFID() {
  while (true) {
    motor_control();
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      handleRFID();
      stop();
      break;
    }
  }
} */
