#include "AGV.h"

AGVState currentState = WAIT_FOR_ROAD;
Coordinate next = { 0, 0 };
Coordinate target = { 0, 0 };
AGV my_agv = { RIGHT, -1, -1 };

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
      turnRight();
      delay(500);
   
      Serial.println("Turn right");
    } else if (turn_direction == LEFT) {
      turnLeft();
      delay(500);

      Serial.println("Turn left");
      
      
    } else if (turn_direction == BACKWARD) {
      turnAround();
      // delay(500); 
      Serial.println("Go backward");
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

void move_forward() {
  motor_control();
  Serial.println("Moving forward");
}


void navigateToNextPosition(String nextPosition) {
  while (nextPosition.indexOf("_") != -1) {
    int underscoreIndex = nextPosition.indexOf("_");
    String token = nextPosition.substring(0, underscoreIndex);
    int numberNext = token.toInt();
    numberToCoordinate(numberNext, next.x, next.y);

    Serial.print("next to: ");
    Serial.print(next.x);
    Serial.print(", ");
    Serial.println(next.y);

    adjust_direction(&my_agv, next);

    while (my_agv.x != next.x || my_agv.y != next.y) {
      move_forward();
      handleRFID();
      delay(100);
    }
    nextPosition = nextPosition.substring(underscoreIndex + 1);
    Serial.println(nextPosition);

    removeProcessedNumberFromFirebase();
  }
}
