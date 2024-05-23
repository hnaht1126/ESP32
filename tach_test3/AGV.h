#ifndef AGV_H
#define AGV_H

#include "FirebaseFunctions.h"
#include "RFIDFunctions.h"
#include "line3Senser.h"

// Directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define BACKWARD 4

// State machine states
enum AGVState {
  WAIT_FOR_ROAD,
  DELIVERING,
  // OBSTACLE_DETECTED,
  RETURN_HOME
};

// Struct representing coordinates
typedef struct {
  int x;
  int y;
} Coordinate;

// Struct representing AGV
typedef struct {
  int direction;  // AGV direction
  int x;          // x-coordinate
  int y;          // y-coordinate
} AGV;

extern AGVState currentState;
extern Coordinate next;
extern Coordinate target;
extern AGV my_agv;

void change_direction(AGV *agv, int new_direction);
int determine_new_direction(AGV *agv, Coordinate next);
void adjust_direction(AGV *agv, Coordinate next);
void move_forward();
void navigateToNextPosition(String nextPosition);

void stop_movement_task();
void start_movement_task();

// void navigateUntilRFID();

#endif
