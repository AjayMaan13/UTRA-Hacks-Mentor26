/*
 * Winter Olympics Robot - Sensor Functions
 * Helper functions for reading and processing sensor data
 */

#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include <Arduino.h>

// ==================== ULTRASONIC SENSOR ====================

float readUltrasonicDistance() {
  // Send trigger pulse
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  // Read echo pulse
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 30000);
  
  // Calculate distance in cm
  float distance = duration * 0.034 / 2;
  
  // Return 0 if out of range
  if (distance > MAX_DISTANCE || distance == 0) {
    return MAX_DISTANCE;
  }
  
  return distance;
}

bool detectObstacle() {
  float distance = readUltrasonicDistance();
  return (distance < OBSTACLE_DISTANCE && distance > 0);
}

// ==================== IR SENSORS ====================

int readIRLeft() {
  return analogRead(IR_LEFT);
}

int readIRRight() {
  return analogRead(IR_RIGHT);
}

bool isOnLine(int sensorValue) {
  return sensorValue < IR_THRESHOLD;
}

// Returns: -1 (left), 0 (center), 1 (right)
int getLinePosition() {
  int leftValue = readIRLeft();
  int rightValue = readIRRight();
  
  bool leftOnLine = isOnLine(leftValue);
  bool rightOnLine = isOnLine(rightValue);
  
  if (leftOnLine && !rightOnLine) {
    return -1;  // Line is to the left
  } else if (!leftOnLine && rightOnLine) {
    return 1;   // Line is to the right
  } else if (leftOnLine && rightOnLine) {
    return 0;   // Centered on line
  } else {
    return 0;   // No line detected, go straight
  }
}

// ==================== COLOR SENSOR ====================

// Set frequency scaling
void setColorSensorScaling(int scale) {
  switch (scale) {
    case 2:   // 2% scaling
      digitalWrite(COLOR_S0, LOW);
      digitalWrite(COLOR_S1, HIGH);
      break;
    case 20:  // 20% scaling (recommended)
      digitalWrite(COLOR_S0, HIGH);
      digitalWrite(COLOR_S1, LOW);
      break;
    case 100: // 100% scaling
      digitalWrite(COLOR_S0, HIGH);
      digitalWrite(COLOR_S1, HIGH);
      break;
    default:  // Power down
      digitalWrite(COLOR_S0, LOW);
      digitalWrite(COLOR_S1, LOW);
      break;
  }
}

// Read red filtered value
int readRed() {
  digitalWrite(COLOR_S2, LOW);
  digitalWrite(COLOR_S3, LOW);
  delay(10);
  return pulseIn(COLOR_OUT, LOW);
}

// Read green filtered value
int readGreen() {
  digitalWrite(COLOR_S2, HIGH);
  digitalWrite(COLOR_S3, HIGH);
  delay(10);
  return pulseIn(COLOR_OUT, LOW);
}

// Read blue filtered value
int readBlue() {
  digitalWrite(COLOR_S2, LOW);
  digitalWrite(COLOR_S3, HIGH);
  delay(10);
  return pulseIn(COLOR_OUT, LOW);
}

// Detect current color
DetectedColor detectColor() {
  int red = readRed();
  int green = readGreen();
  int blue = readBlue();
  
  // Check for black (center target)
  if (red >= BLACK_R_MIN && red <= BLACK_R_MAX &&
      green >= BLACK_G_MIN && green <= BLACK_G_MAX &&
      blue >= BLACK_B_MIN && blue <= BLACK_B_MAX) {
    return COLOR_BLACK;
  }
  
  // Check for green (inner ring)
  if (red >= GREEN_R_MIN && red <= GREEN_R_MAX &&
      green >= GREEN_G_MIN && green <= GREEN_G_MAX &&
      blue >= GREEN_B_MIN && blue <= GREEN_B_MAX) {
    return COLOR_GREEN;
  }
  
  // Check for red (middle ring / obstacle path)
  if (red >= RED_R_MIN && red <= RED_R_MAX &&
      green >= RED_G_MIN && green <= RED_G_MAX &&
      blue >= RED_B_MIN && blue <= RED_B_MAX) {
    return COLOR_RED;
  }
  
  // Check for blue (outer ring / path markers)
  if (red >= BLUE_R_MIN && red <= BLUE_R_MAX &&
      green >= BLUE_G_MIN && green <= BLUE_G_MAX &&
      blue >= BLUE_B_MIN && blue <= BLUE_B_MAX) {
    return COLOR_BLUE;
  }
  
  // Check for white (background)
  if (red >= WHITE_R_MIN && red <= WHITE_R_MAX &&
      green >= WHITE_G_MIN && green <= WHITE_G_MAX &&
      blue >= WHITE_B_MIN && blue <= WHITE_B_MAX) {
    return COLOR_WHITE;
  }
  
  return COLOR_UNKNOWN;
}

// Get color name as string (for debugging)
const char* getColorName(DetectedColor color) {
  switch (color) {
    case COLOR_BLACK:   return "Black";
    case COLOR_GREEN:   return "Green";
    case COLOR_RED:     return "Red";
    case COLOR_BLUE:    return "Blue";
    case COLOR_WHITE:   return "White";
    default:            return "Unknown";
  }
}

// ==================== SENSOR INITIALIZATION ====================

void initializeSensors() {
  // Ultrasonic sensor
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  
  // IR sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  
  // Color sensor
  pinMode(COLOR_S0, OUTPUT);
  pinMode(COLOR_S1, OUTPUT);
  pinMode(COLOR_S2, OUTPUT);
  pinMode(COLOR_S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);
  
  // Set color sensor scaling to 20%
  setColorSensorScaling(20);
  
  Serial.println("Sensors initialized");
}

#endif // SENSORS_H
