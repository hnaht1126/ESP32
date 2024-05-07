// agv_controller.h
#ifndef AGV_CONTROLLER_H
#define AGV_CONTROLLER_H

#include <Arduino.h>

typedef struct {
  int direction;
  int x;
  int y;
} AGV;

void initializeAGV(AGV *agv);
void change_direction(AGV *agv, int new_direction);
void move_forward(AGV *agv);
int determine_new_direction(int current_x, int current_y, int next_x, int next_y);
void adjust_direction(AGV *agv, int next_x, int next_y);
void numberToCoordinate(int number, int &x, int &y);
// void navigateToNextPosition(String nextPosition);

#endif