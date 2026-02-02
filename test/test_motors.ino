/*
 * Motor Test Sketch
 * Tests all motor movements to verify proper operation
 */

// Pin definitions
#define MOTOR_LEFT_IN1    7
#define MOTOR_LEFT_IN2    6
#define MOTOR_LEFT_EN     5
#define MOTOR_RIGHT_IN3   4
#define MOTOR_RIGHT_IN4   3
#define MOTOR_RIGHT_EN    2

#define TEST_SPEED        180
#define TEST_DURATION     2000  // ms

void setup() {
  Serial.begin(9600);
  Serial.println("=== Motor Test ===");
  
  // Initialize pins
  pinMode(MOTOR_LEFT_IN1, OUTPUT);
  pinMode(MOTOR_LEFT_IN2, OUTPUT);
  pinMode(MOTOR_LEFT_EN, OUTPUT);
  pinMode(MOTOR_RIGHT_IN3, OUTPUT);
  pinMode(MOTOR_RIGHT_IN4, OUTPUT);
  pinMode(MOTOR_RIGHT_EN, OUTPUT);
  
  Serial.println("Starting test in 3 seconds...");
  delay(3000);
}

void loop() {
  Serial.println("\n1. Testing FORWARD movement");
  moveForward();
  delay(TEST_DURATION);
  stopMotors();
  delay(1000);
  
  Serial.println("2. Testing BACKWARD movement");
  moveBackward();
  delay(TEST_DURATION);
  stopMotors();
  delay(1000);
  
  Serial.println("3. Testing LEFT turn");
  turnLeft();
  delay(TEST_DURATION);
  stopMotors();
  delay(1000);
  
  Serial.println("4. Testing RIGHT turn");
  turnRight();
  delay(TEST_DURATION);
  stopMotors();
  delay(1000);
  
  Serial.println("\n=== Test complete ===");
  Serial.println("All movements OK? (Y/N)");
  Serial.println("Repeating in 5 seconds...\n");
  delay(5000);
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, TEST_SPEED);
  
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, TEST_SPEED);
}

void moveBackward() {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_EN, TEST_SPEED);
  
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  analogWrite(MOTOR_RIGHT_EN, TEST_SPEED);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, HIGH);
  analogWrite(MOTOR_LEFT_EN, TEST_SPEED);
  
  digitalWrite(MOTOR_RIGHT_IN3, HIGH);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, TEST_SPEED);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_IN1, HIGH);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, TEST_SPEED);
  
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, HIGH);
  analogWrite(MOTOR_RIGHT_EN, TEST_SPEED);
}

void stopMotors() {
  digitalWrite(MOTOR_LEFT_IN1, LOW);
  digitalWrite(MOTOR_LEFT_IN2, LOW);
  analogWrite(MOTOR_LEFT_EN, 0);
  
  digitalWrite(MOTOR_RIGHT_IN3, LOW);
  digitalWrite(MOTOR_RIGHT_IN4, LOW);
  analogWrite(MOTOR_RIGHT_EN, 0);
}
