/*
 * Motor Calibration Sketch
 * Use this to calibrate motor speeds for straight-line movement
 * 
 * Instructions:
 * 1. Upload this sketch
 * 2. Open Serial Monitor (9600 baud)
 * 3. Place robot on flat surface with wheels elevated
 * 4. Observe which motor spins faster
 * 5. Adjust MOTOR_LEFT_SPEED or MOTOR_RIGHT_SPEED in config.h
 * 6. Re-upload and test until wheels spin at equal speeds
 */

// Pin definitions (same as config.h)
#define MOTOR_LEFT_IN1    7
#define MOTOR_LEFT_IN2    6
#define MOTOR_LEFT_EN     5
#define MOTOR_RIGHT_IN3   4
#define MOTOR_RIGHT_IN4   3
#define MOTOR_RIGHT_EN    2

// Test speeds (adjust these)
int leftSpeed = 200;
int rightSpeed = 200;

void setup() {
  Serial.begin(9600);
  Serial.println("=== Motor Calibration ===");
  Serial.println("Calibrating motor speeds for straight-line movement");
  Serial.println();
  
  // Initialize motor pins
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);
  pinMode(MOTOR_RIGHT_IN3, OUTPUT);
  pinMode(MOTOR_RIGHT_IN4, OUTPUT);
  pinMode(MOTOR_RIGHT_EN, OUTPUT);
  
  Serial.println("Waiting 3 seconds before starting...");
  delay(3000);
}

void loop() {
  // Test forward motion
  Serial.println("\n--- Testing FORWARD motion ---");
  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);
  
  moveForward(leftSpeed, rightSpeed);
  delay(3000);
  stopMotors();
  delay(2000);
  
  // Test backward motion
  Serial.println("\n--- Testing BACKWARD motion ---");
  Serial.print("Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);
  
  moveBackward(leftSpeed, rightSpeed);
  delay(3000);
  stopMotors();
  delay(2000);
  
  // Test left turn
  Serial.println("\n--- Testing LEFT turn ---");
  turnLeft(150);
  delay(2000);
  stopMotors();
  delay(2000);
  
  // Test right turn
  Serial.println("\n--- Testing RIGHT turn ---");
  turnRight(150);
  delay(2000);
  stopMotors();
  delay(2000);
  
  Serial.println("\n======================");
  Serial.println("Cycle complete. Repeating...");
  Serial.println("If robot veers left: INCREASE right speed or DECREASE left speed");
  Serial.println("If robot veers right: INCREASE left speed or DECREASE right speed");
  Serial.println("======================\n");
  
  delay(5000);
}

void moveForward(int leftSpd, int rightSpd) {
  // Left motor forward
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, leftSpd);
  
  // Right motor forward
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, rightSpd);
}

void moveBackward(int leftSpd, int rightSpd) {
  // Left motor backward
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_EN, leftSpd);
  
  // Right motor backward
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  analogWrite(MOTOR_RIGHT_EN, rightSpd);
}

void turnLeft(int speed) {
  // Left motor backward
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_EN, speed);
  
  // Right motor forward
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, speed);
}

void turnRight(int speed) {
  // Left motor forward
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, speed);
  
  // Right motor backward
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
