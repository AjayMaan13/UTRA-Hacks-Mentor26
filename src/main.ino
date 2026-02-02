/*
 * Winter Olympics Robot - Main Control Program
 * Autonomous navigation, color detection, and ball shooting
 * 
 * Features:
 * - Box pickup and placement
 * - Path detection (red/green)
 * - Line following with IR sensors
 * - Color-based target navigation
 * - Obstacle avoidance
 * - Ball shooting mechanism
 * 
 * Team: Seneca Peer Mentors
 * Event: Winter Olympics Hackathon 2025
 */

#include <Servo.h>
#include "config.h"
#include "sensors.h"

// Servo objects
Servo clawServo;
Servo shooterServo;

// Robot state
RobotState currentState = STATE_INIT;
DetectedColor targetPath = COLOR_UNKNOWN;

void setup() {
  Serial.begin(9600);
  Serial.println("=== Winter Olympics Robot ===");
  Serial.println("Initializing...");
  
  // Initialize motors
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);
  pinMode(MOTOR_RIGHT_IN3, OUTPUT);
  pinMode(MOTOR_RIGHT_IN4, OUTPUT);
  pinMode(MOTOR_RIGHT_EN, OUTPUT);
  
  // Initialize servos
  clawServo.attach(SERVO_CLAW);
  shooterServo.attach(SERVO_SHOOTER);
  clawServo.write(CLAW_OPEN);
  shooterServo.write(SHOOTER_READY);
  
  // Initialize sensors
  initializeSensors();
  
  Serial.println("Initialization complete!");
  Serial.println("Starting autonomous operation in 3 seconds...\n");
  delay(3000);
  
  currentState = STATE_PICKUP_BOX;
}

void loop() {
  switch (currentState) {
    case STATE_PICKUP_BOX:
      handlePickupBox();
      break;
      
    case STATE_DETECT_PATH:
      handleDetectPath();
      break;
      
    case STATE_FOLLOW_PATH:
      handleFollowPath();
      break;
      
    case STATE_CLIMB_RAMP:
      handleClimbRamp();
      break;
      
    case STATE_NAVIGATE_TARGET:
      handleNavigateTarget();
      break;
      
    case STATE_SHOOT_BALL:
      handleShootBall();
      break;
      
    case STATE_RETURN:
      handleReturn();
      break;
      
    case STATE_OBSTACLE_COURSE:
      handleObstacleCourse();
      break;
      
    case STATE_COMPLETE:
      handleComplete();
      break;
      
    default:
      Serial.println("ERROR: Unknown state");
      stopMotors();
      break;
  }
  
  delay(CONTROL_LOOP_DELAY);
}

// ==================== STATE HANDLERS ====================

void handlePickupBox() {
  Serial.println("STATE: Picking up box");
  
  // Move forward slowly to box
  moveForward(SLOW_SPEED, SLOW_SPEED);
  delay(1000);
  stopMotors();
  
  // Close claw to grip box
  clawServo.write(CLAW_CLOSED);
  delay(PICKUP_DELAY);
  
  Serial.println("Box picked up");
  currentState = STATE_DETECT_PATH;
}

void handleDetectPath() {
  Serial.println("STATE: Detecting path color");
  
  // Move forward to path split
  moveForward(SLOW_SPEED, SLOW_SPEED);
  delay(500);
  
  // Read color at path split
  DetectedColor pathColor = detectColor();
  Serial.print("Detected path color: ");
  Serial.println(getColorName(pathColor));
  
  // Place box in appropriate zone
  if (pathColor == COLOR_RED || pathColor == COLOR_GREEN) {
    targetPath = pathColor;
    
    // Open claw to release box
    clawServo.write(CLAW_OPEN);
    delay(500);
    
    Serial.print("Box placed on ");
    Serial.print(getColorName(targetPath));
    Serial.println(" path");
    
    currentState = STATE_FOLLOW_PATH;
  } else {
    // Color not recognized, retry
    Serial.println("Path color unclear, retrying...");
    moveBackward(SLOW_SPEED, SLOW_SPEED);
    delay(300);
    stopMotors();
    delay(500);
  }
}

void handleFollowPath() {
  Serial.println("STATE: Following path");
  
  // Follow line using IR sensors
  int linePosition = getLinePosition();
  
  if (linePosition == 0) {
    // Centered on line
    moveForward(BASE_SPEED, BASE_SPEED);
  } else if (linePosition == -1) {
    // Line to the left, turn left
    turnLeft(TURN_SPEED);
  } else if (linePosition == 1) {
    // Line to the right, turn right
    turnRight(TURN_SPEED);
  }
  
  // Check for obstacles (if on obstacle course)
  if (targetPath == COLOR_RED) {
    if (detectObstacle()) {
      Serial.println("Obstacle detected! Avoiding...");
      avoidObstacle();
    }
  }
  
  // Check if reached ramp or target
  float distance = readUltrasonicDistance();
  if (distance < RAMP_DISTANCE) {
    stopMotors();
    
    if (targetPath == COLOR_GREEN) {
      currentState = STATE_CLIMB_RAMP;
    } else {
      currentState = STATE_OBSTACLE_COURSE;
    }
  }
}

void handleClimbRamp() {
  Serial.println("STATE: Climbing ramp");
  
  // Increase speed for ramp climbing
  moveForward(255, 255);  // Max speed
  delay(2000);
  
  stopMotors();
  Serial.println("Reached top of ramp");
  
  currentState = STATE_NAVIGATE_TARGET;
}

void handleNavigateTarget() {
  Serial.println("STATE: Navigating to target center");
  
  // Read current ring color
  DetectedColor ringColor = detectColor();
  Serial.print("Current ring: ");
  Serial.println(getColorName(ringColor));
  
  if (ringColor == COLOR_BLACK) {
    // Reached center!
    stopMotors();
    Serial.println("Reached BLACK center!");
    currentState = STATE_SHOOT_BALL;
  } else {
    // Move toward center based on color
    navigateRing(ringColor);
  }
}

void navigateRing(DetectedColor ring) {
  // Simple strategy: move forward until reaching black
  switch (ring) {
    case COLOR_BLUE:
      Serial.println("On BLUE ring, moving inward");
      moveForward(BASE_SPEED, BASE_SPEED);
      break;
      
    case COLOR_RED:
      Serial.println("On RED ring, moving inward");
      moveForward(BASE_SPEED, BASE_SPEED);
      break;
      
    case COLOR_GREEN:
      Serial.println("On GREEN ring, moving inward");
      moveForward(SLOW_SPEED, SLOW_SPEED);
      break;
      
    default:
      // If lost, try moving forward slowly
      moveForward(SLOW_SPEED, SLOW_SPEED);
      break;
  }
}

void handleShootBall() {
  Serial.println("STATE: Shooting ball");
  
  stopMotors();
  delay(500);
  
  // Shoot ball
  shooterServo.write(SHOOTER_SHOOT);
  delay(SHOOT_DELAY);
  
  // Reset shooter
  shooterServo.write(SHOOTER_RESET);
  delay(500);
  shooterServo.write(SHOOTER_READY);
  
  Serial.println("Ball shot!");
  currentState = STATE_RETURN;
}

void handleReturn() {
  Serial.println("STATE: Returning to start");
  
  // Turn around
  turnAround();
  
  // Follow path back
  for (int i = 0; i < 100; i++) {  // Max iterations
    int linePosition = getLinePosition();
    
    if (linePosition == 0) {
      moveForward(BASE_SPEED, BASE_SPEED);
    } else if (linePosition == -1) {
      turnLeft(TURN_SPEED);
    } else {
      turnRight(TURN_SPEED);
    }
    
    delay(50);
    
    // Check if reached start (could use distance or color marker)
    if (detectColor() == COLOR_BLUE) {
      break;
    }
  }
  
  stopMotors();
  Serial.println("Returned to start area");
  currentState = STATE_COMPLETE;
}

void handleObstacleCourse() {
  Serial.println("STATE: Obstacle course");
  
  // Follow path and avoid obstacles
  int linePosition = getLinePosition();
  
  if (detectObstacle()) {
    Serial.println("Obstacle! Avoiding...");
    avoidObstacle();
  } else if (linePosition == 0) {
    moveForward(BASE_SPEED, BASE_SPEED);
  } else if (linePosition == -1) {
    turnLeft(TURN_SPEED);
  } else {
    turnRight(TURN_SPEED);
  }
  
  // Check if course complete (reached starting area)
  if (detectColor() == COLOR_BLUE) {
    stopMotors();
    currentState = STATE_COMPLETE;
  }
}

void handleComplete() {
  Serial.println("STATE: Complete!");
  Serial.println("Course finished. Robot stopped.");
  stopMotors();
  
  // Infinite loop to prevent further movement
  while (true) {
    delay(1000);
  }
}

// ==================== MOTOR FUNCTIONS ====================

void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, leftSpeed);
  
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, rightSpeed);
}

void moveBackward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_EN, leftSpeed);
  
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  analogWrite(MOTOR_RIGHT_EN, rightSpeed);
}

void turnLeft(int speed) {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_EN, speed);
  
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, speed);
}

void turnRight(int speed) {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, speed);
  
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  analogWrite(MOTOR_RIGHT_EN, speed);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, 0);
  
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, 0);
}

void turnAround() {
  turnRight(TURN_SPEED);
  delay(TURN_DELAY * 2);  // 180 degree turn
  stopMotors();
}

void avoidObstacle() {
  // Stop
  stopMotors();
  delay(200);
  
  // Back up
  moveBackward(SLOW_SPEED, SLOW_SPEED);
  delay(500);
  stopMotors();
  delay(200);
  
  // Turn right
  turnRight(TURN_SPEED);
  delay(TURN_DELAY / 2);
  stopMotors();
  delay(200);
  
  // Move forward to get around obstacle
  moveForward(BASE_SPEED, BASE_SPEED);
  delay(1000);
  
  // Turn left to get back on path
  turnLeft(TURN_SPEED);
  delay(TURN_DELAY / 2);
  stopMotors();
}
