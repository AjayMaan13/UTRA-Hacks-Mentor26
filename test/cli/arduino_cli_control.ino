/*
 * Arduino Multi-Sensor CLI Control System
 * Serial-based command line interface to control all components
 * 
 * Commands:
 *   help - Show all commands
 *   color - Read color sensor
 *   motor <A/B> <speed> <dir> - Control motor (speed: 0-255, dir: F/B)
 *   motors <speed> <dir> - Control both motors
 *   stop - Stop all motors
 *   servo <1/2> <angle> - Set servo position (angle: 0-180)
 *   ir - Read IR sensors
 *   ultra - Read ultrasonic sensor
 *   status - Show all sensor readings
 */

#include <Servo.h>

// TCS3200 Color Sensor pins
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define SENSOR_OUT 6
#define COLOR_LED 7

// L293D Motor Driver pins
#define ENABLE_A 8
#define INPUT_1 9
#define INPUT_2 10
#define ENABLE_B 11
#define INPUT_3 12
#define INPUT_4 13

// Servo pins
#define SERVO1_PIN A3
#define SERVO2_PIN A1

// IR sensor pins
#define IR1_PIN A2
#define IR2_PIN A0

// Ultrasonic sensor pins
#define TRIG_PIN A4
#define ECHO_PIN A5

// Servo objects
Servo servo1;
Servo servo2;

// Variables
String inputString = "";
boolean stringComplete = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize TCS3200
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(COLOR_LED, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(COLOR_LED, HIGH);
  
  // Initialize L293D
  pinMode(ENABLE_A, OUTPUT);
  pinMode(INPUT_1, OUTPUT);
  pinMode(INPUT_2, OUTPUT);
  pinMode(ENABLE_B, OUTPUT);
  pinMode(INPUT_3, OUTPUT);
  pinMode(INPUT_4, OUTPUT);
  
  // Initialize Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(90);
  servo2.write(90);
  
  // Initialize IR sensors
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  
  // Initialize Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Stop motors initially
  analogWrite(ENABLE_A, 0);
  analogWrite(ENABLE_B, 0);
  
  // Reserve string buffer
  inputString.reserve(50);
  
  // Display welcome message
  Serial.println("========================================");
  Serial.println("  Arduino Multi-Sensor Control System  ");
  Serial.println("========================================");
  Serial.println();
  printHelp();
  Serial.println();
  Serial.print("> ");
}

void loop() {
  // Check for serial input
  if (stringComplete) {
    inputString.trim();
    processCommand(inputString);
    inputString = "";
    stringComplete = false;
    Serial.print("> ");
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

void processCommand(String cmd) {
  cmd.toLowerCase();
  
  if (cmd == "help") {
    printHelp();
  }
  else if (cmd == "color") {
    readColorSensor();
  }
  else if (cmd.startsWith("motor ")) {
    controlMotor(cmd);
  }
  else if (cmd.startsWith("motors ")) {
    controlBothMotors(cmd);
  }
  else if (cmd == "stop") {
    stopMotors();
  }
  else if (cmd.startsWith("servo ")) {
    controlServo(cmd);
  }
  else if (cmd == "ir") {
    readIRSensors();
  }
  else if (cmd == "ultra") {
    readUltrasonic();
  }
  else if (cmd == "status") {
    showAllStatus();
  }
  else {
    Serial.println("Unknown command. Type 'help' for available commands.");
  }
}

void printHelp() {
  Serial.println("Available Commands:");
  Serial.println("------------------");
  Serial.println("  help                      - Show this help message");
  Serial.println("  color                     - Read TCS3200 color sensor");
  Serial.println("  motor <A/B> <speed> <dir> - Control motor (speed: 0-255, dir: F/B)");
  Serial.println("                              Example: motor A 200 F");
  Serial.println("  motors <speed> <dir>      - Control both motors");
  Serial.println("                              Example: motors 150 B");
  Serial.println("  stop                      - Stop all motors");
  Serial.println("  servo <1/2> <angle>       - Set servo position (angle: 0-180)");
  Serial.println("                              Example: servo 1 90");
  Serial.println("  ir                        - Read IR sensors");
  Serial.println("  ultra                     - Read ultrasonic distance");
  Serial.println("  status                    - Show all sensor readings");
}

void readColorSensor() {
  Serial.println("Reading Color Sensor...");
  
  // Read RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int red = pulseIn(SENSOR_OUT, LOW);
  delay(50);
  
  // Read GREEN
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int green = pulseIn(SENSOR_OUT, LOW);
  delay(50);
  
  // Read BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int blue = pulseIn(SENSOR_OUT, LOW);
  
  Serial.print("  R: ");
  Serial.print(red);
  Serial.print("  G: ");
  Serial.print(green);
  Serial.print("  B: ");
  Serial.println(blue);
}

void controlMotor(String cmd) {
  // Parse: motor <A/B> <speed> <dir>
  int firstSpace = cmd.indexOf(' ');
  int secondSpace = cmd.indexOf(' ', firstSpace + 1);
  int thirdSpace = cmd.indexOf(' ', secondSpace + 1);
  
  if (secondSpace == -1 || thirdSpace == -1) {
    Serial.println("Error: Format is 'motor <A/B> <speed> <dir>'");
    return;
  }
  
  char motor = cmd.charAt(firstSpace + 1);
  int speed = cmd.substring(secondSpace + 1, thirdSpace).toInt();
  char dir = cmd.charAt(thirdSpace + 1);
  
  if (speed < 0 || speed > 255) {
    Serial.println("Error: Speed must be 0-255");
    return;
  }
  
  if (motor == 'a') {
    if (dir == 'f') {
      digitalWrite(INPUT_1, HIGH);
      digitalWrite(INPUT_2, LOW);
    } else if (dir == 'b') {
      digitalWrite(INPUT_1, LOW);
      digitalWrite(INPUT_2, HIGH);
    } else {
      Serial.println("Error: Direction must be F or B");
      return;
    }
    analogWrite(ENABLE_A, speed);
    Serial.print("Motor A set to ");
    Serial.print(speed);
    Serial.print(" ");
    Serial.println(dir == 'f' ? "Forward" : "Backward");
  }
  else if (motor == 'b') {
    if (dir == 'f') {
      digitalWrite(INPUT_3, HIGH);
      digitalWrite(INPUT_4, LOW);
    } else if (dir == 'b') {
      digitalWrite(INPUT_3, LOW);
      digitalWrite(INPUT_4, HIGH);
    } else {
      Serial.println("Error: Direction must be F or B");
      return;
    }
    analogWrite(ENABLE_B, speed);
    Serial.print("Motor B set to ");
    Serial.print(speed);
    Serial.print(" ");
    Serial.println(dir == 'f' ? "Forward" : "Backward");
  }
  else {
    Serial.println("Error: Motor must be A or B");
  }
}

void controlBothMotors(String cmd) {
  // Parse: motors <speed> <dir>
  int firstSpace = cmd.indexOf(' ');
  int secondSpace = cmd.indexOf(' ', firstSpace + 1);
  
  if (secondSpace == -1) {
    Serial.println("Error: Format is 'motors <speed> <dir>'");
    return;
  }
  
  int speed = cmd.substring(firstSpace + 1, secondSpace).toInt();
  char dir = cmd.charAt(secondSpace + 1);
  
  if (speed < 0 || speed > 255) {
    Serial.println("Error: Speed must be 0-255");
    return;
  }
  
  if (dir == 'f') {
    digitalWrite(INPUT_1, HIGH);
    digitalWrite(INPUT_2, LOW);
    digitalWrite(INPUT_3, HIGH);
    digitalWrite(INPUT_4, LOW);
  } else if (dir == 'b') {
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    digitalWrite(INPUT_3, LOW);
    digitalWrite(INPUT_4, HIGH);
  } else {
    Serial.println("Error: Direction must be F or B");
    return;
  }
  
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  Serial.print("Both motors set to ");
  Serial.print(speed);
  Serial.print(" ");
  Serial.println(dir == 'f' ? "Forward" : "Backward");
}

void stopMotors() {
  analogWrite(ENABLE_A, 0);
  analogWrite(ENABLE_B, 0);
  Serial.println("All motors stopped");
}

void controlServo(String cmd) {
  // Parse: servo <1/2> <angle>
  int firstSpace = cmd.indexOf(' ');
  int secondSpace = cmd.indexOf(' ', firstSpace + 1);
  
  if (secondSpace == -1) {
    Serial.println("Error: Format is 'servo <1/2> <angle>'");
    return;
  }
  
  int servoNum = cmd.charAt(firstSpace + 1) - '0';
  int angle = cmd.substring(secondSpace + 1).toInt();
  
  if (angle < 0 || angle > 180) {
    Serial.println("Error: Angle must be 0-180");
    return;
  }
  
  if (servoNum == 1) {
    servo1.write(angle);
    Serial.print("Servo 1 set to ");
    Serial.print(angle);
    Serial.println(" degrees");
  }
  else if (servoNum == 2) {
    servo2.write(angle);
    Serial.print("Servo 2 set to ");
    Serial.print(angle);
    Serial.println(" degrees");
  }
  else {
    Serial.println("Error: Servo must be 1 or 2");
  }
}

void readIRSensors() {
  Serial.println("IR Sensor Readings:");
  int ir1 = digitalRead(IR1_PIN);
  int ir2 = digitalRead(IR2_PIN);
  
  Serial.print("  IR1: ");
  Serial.print(ir1);
  Serial.println(ir1 == LOW ? " [DETECTED]" : " [CLEAR]");
  
  Serial.print("  IR2: ");
  Serial.print(ir2);
  Serial.println(ir2 == LOW ? " [DETECTED]" : " [CLEAR]");
}

void readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distanceCm = duration * 0.0343 / 2;
  float distanceInch = distanceCm / 2.54;
  
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm  (");
  Serial.print(distanceInch);
  Serial.println(" inches)");
  
  if (distanceCm > 400 || distanceCm < 2) {
    Serial.println("  Warning: Out of range (2-400 cm)");
  }
}

void showAllStatus() {
  Serial.println("========== SYSTEM STATUS ==========");
  Serial.println();
  
  Serial.println("COLOR SENSOR:");
  readColorSensor();
  Serial.println();
  
  Serial.println("IR SENSORS:");
  readIRSensors();
  Serial.println();
  
  Serial.println("ULTRASONIC SENSOR:");
  readUltrasonic();
  Serial.println();
  
  Serial.println("===================================");
}
