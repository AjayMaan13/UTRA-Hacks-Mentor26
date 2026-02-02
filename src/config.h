/*
 * Winter Olympics Robot - Configuration File
 * Pin definitions and calibration constants
 */

#ifndef CONFIG_H
#define CONFIG_H

// ==================== PIN DEFINITIONS ====================

// Motor Driver (L298N) Pins
#define MOTOR_LEFT_IN1    7
#define MOTOR_LEFT_IN2    6
#define MOTOR_LEFT_EN     5   // PWM pin
#define MOTOR_RIGHT_IN3   4
#define MOTOR_RIGHT_IN4   3
#define MOTOR_RIGHT_EN    2   // PWM pin

// Servo Motor Pins
#define SERVO_CLAW        9   // Claw servo
#define SERVO_SHOOTER     10  // Ball shooter servo

// Ultrasonic Sensor (HC-SR04) Pins
#define ULTRASONIC_TRIG   12
#define ULTRASONIC_ECHO   11

// IR Sensors Pins
#define IR_LEFT           A0
#define IR_RIGHT          A1

// Color Sensor (TCS3200) Pins
#define COLOR_S0          8
#define COLOR_S1          13
#define COLOR_S2          A2
#define COLOR_S3          A3
#define COLOR_OUT         A4

// ==================== CALIBRATION VALUES ====================
// Update these after running calibration sketches!

// Motor Speed Calibration (0-255)
#define MOTOR_LEFT_SPEED   200
#define MOTOR_RIGHT_SPEED  195
#define BASE_SPEED         180
#define TURN_SPEED         120
#define SLOW_SPEED         100

// IR Sensor Thresholds
#define IR_THRESHOLD       500  // Adjust based on calibration
#define IR_WHITE_MIN       700
#define IR_BLACK_MAX       300

// Ultrasonic Sensor Settings
#define OBSTACLE_DISTANCE  20   // cm - distance to detect obstacles
#define RAMP_DISTANCE      10   // cm - distance to detect ramp
#define MAX_DISTANCE       200  // cm - maximum reliable range

// Color Sensor Thresholds (RGB values)
// Calibrate these by pointing sensor at each color!

// Black (center target)
#define BLACK_R_MIN        0
#define BLACK_R_MAX        50
#define BLACK_G_MIN        0
#define BLACK_G_MAX        50
#define BLACK_B_MIN        0
#define BLACK_B_MAX        50

// Green (inner ring)
#define GREEN_R_MIN        50
#define GREEN_R_MAX        100
#define GREEN_G_MIN        100
#define GREEN_G_MAX        200
#define GREEN_B_MIN        50
#define GREEN_B_MAX        100

// Red (middle ring)
#define RED_R_MIN          150
#define RED_R_MAX          255
#define RED_G_MIN          0
#define RED_G_MAX          80
#define RED_B_MIN          0
#define RED_B_MAX          80

// Blue (outer ring)
#define BLUE_R_MIN         0
#define BLUE_R_MAX         80
#define BLUE_G_MIN         0
#define BLUE_G_MAX         100
#define BLUE_B_MIN         150
#define BLUE_B_MAX         255

// White (background)
#define WHITE_R_MIN        200
#define WHITE_R_MAX        255
#define WHITE_G_MIN        200
#define WHITE_G_MAX        255
#define WHITE_B_MIN        200
#define WHITE_B_MAX        255

// ==================== SERVO POSITIONS ====================

// Claw positions (degrees)
#define CLAW_OPEN          90
#define CLAW_CLOSED        0
#define CLAW_HOLD          45

// Shooter positions (degrees)
#define SHOOTER_READY      0
#define SHOOTER_SHOOT      180
#define SHOOTER_RESET      90

// ==================== TIMING CONSTANTS ====================

#define SENSOR_READ_DELAY  50   // ms between sensor readings
#define CONTROL_LOOP_DELAY 10   // ms per control loop iteration
#define TURN_DELAY         500  // ms for 90-degree turn
#define PICKUP_DELAY       1000 // ms for box pickup
#define SHOOT_DELAY        500  // ms for shooting motion

// ==================== PID CONSTANTS ====================
// For advanced line following

#define KP                 1.0  // Proportional gain
#define KI                 0.0  // Integral gain
#define KD                 0.5  // Derivative gain

// ==================== ROBOT STATES ====================

enum RobotState {
  STATE_INIT,
  STATE_PICKUP_BOX,
  STATE_DETECT_PATH,
  STATE_FOLLOW_PATH,
  STATE_CLIMB_RAMP,
  STATE_NAVIGATE_TARGET,
  STATE_SHOOT_BALL,
  STATE_RETURN,
  STATE_OBSTACLE_COURSE,
  STATE_COMPLETE
};

// ==================== COLOR DEFINITIONS ====================

enum DetectedColor {
  COLOR_BLACK,
  COLOR_GREEN,
  COLOR_RED,
  COLOR_BLUE,
  COLOR_WHITE,
  COLOR_UNKNOWN
};

#endif // CONFIG_H
