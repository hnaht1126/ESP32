#ifndef LINE3SENSER_H
#define LINE3SENSER_H

#include <Arduino.h>


/* ------------------------------------------------------------------------- */
/*                                 DEFINE PIN                                */
/* ------------------------------------------------------------------------- */
/**
 * Pin kết nối Thanh dò line TCRT5000
 *
 * TCRT5000 : ESP32
 * GND      - GND
 * 5V       - 5V
 * OUT1     - 32 (Digital)
 * OUT2     - 35 (Digital)
 * OUT3     - 34 (Digital)
 */
#define PIN_OUT1 32 //! D32 //trai
#define PIN_OUT2 35 //! D35
#define PIN_OUT3 34 //! D34 //phai

#define IR_SENSOR_PIN 33

/* ------------------------------------------------------------------------- */

/**
 * Pin kết nối Driver L298
 *
 * Phần cấp nguồn cho Driver
 * +12V - ... có thể cấp nguồn trong khoảng 9V~6V
 * GND  - GND
 *
 * Dùng Jumper kết nối (mặc định)
 * ENA  - 5V
 * ENB  - 5V
 *
 * L298 : ESP32
 * IN1  : 12
 * IN2  : 14 (~)
 * IN3  : 27 (~)
 * IN4  : 26
 */
#define PIN_IN1 12 //! D12
#define PIN_IN2 14 //! D14 (~)
#define PIN_IN3 27 //! D27 (~)
#define PIN_IN4 26 //! D26


#define PIN_ENA 13 //! D13
#define PIN_ENB 25 //! D25


/* ------------------------------------------------------------------------- */
/*                                  FUNCTION                                 */
/* ------------------------------------------------------------------------- */
void motor_control();

/* ------------------------- Điều khiển xe dừng lại ------------------------ */
void stop();

/* ------------------- Điều khiển xe di chuyển tùy chỉnh ------------------- */
void moveForward();
void turnAround();
void turnLeft();
void turnRight();

void setupDriver();

#endif
