/*
 * Servo Motor Test Sketch
 * Tests claw and shooter servos through their full range
 */

#include <Servo.h>

#define SERVO_CLAW     9
#define SERVO_SHOOTER  10

Servo clawServo;
Servo shooterServo;

void setup() {
  Serial.begin(9600);
  Serial.println("=== Servo Test ===");
  
  // Attach servos
  clawServo.attach(SERVO_CLAW);
  shooterServo.attach(SERVO_SHOOTER);
  
  // Initialize to neutral positions
  clawServo.write(90);
  shooterServo.write(90);
  
  Serial.println("Servos initialized to 90 degrees");
  Serial.println("Starting test in 2 seconds...\n");
  delay(2000);
}

void loop() {
  // Test claw servo
  Serial.println("=== Testing CLAW Servo ===");
  
  Serial.println("Claw OPEN (90 degrees)");
  clawServo.write(90);
  delay(2000);
  
  Serial.println("Claw CLOSING (moving to 0 degrees)");
  for (int pos = 90; pos >= 0; pos--) {
    clawServo.write(pos);
    delay(15);
  }
  delay(1000);
  
  Serial.println("Claw CLOSED (0 degrees)");
  delay(2000);
  
  Serial.println("Claw OPENING (moving to 90 degrees)");
  for (int pos = 0; pos <= 90; pos++) {
    clawServo.write(pos);
    delay(15);
  }
  delay(1000);
  
  Serial.println("Claw test complete\n");
  delay(2000);
  
  // Test shooter servo
  Serial.println("=== Testing SHOOTER Servo ===");
  
  Serial.println("Shooter READY (0 degrees)");
  shooterServo.write(0);
  delay(2000);
  
  Serial.println("Shooter SHOOTING (moving to 180 degrees)");
  for (int pos = 0; pos <= 180; pos++) {
    shooterServo.write(pos);
    delay(10);
  }
  delay(1000);
  
  Serial.println("Shooter at FULL extension (180 degrees)");
  delay(2000);
  
  Serial.println("Shooter RESETTING (moving to 90 degrees)");
  for (int pos = 180; pos >= 90; pos--) {
    shooterServo.write(pos);
    delay(10);
  }
  delay(1000);
  
  Serial.println("Shooter test complete\n");
  delay(2000);
  
  // Sweep test for both servos
  Serial.println("=== SWEEP Test (both servos) ===");
  for (int i = 0; i < 3; i++) {
    Serial.print("Sweep ");
    Serial.println(i + 1);
    
    for (int pos = 0; pos <= 180; pos += 5) {
      clawServo.write(pos);
      shooterServo.write(180 - pos);  // Move in opposite direction
      delay(30);
    }
    
    for (int pos = 180; pos >= 0; pos -= 5) {
      clawServo.write(pos);
      shooterServo.write(180 - pos);
      delay(30);
    }
  }
  
  // Return to neutral
  Serial.println("\nReturning to neutral positions...");
  clawServo.write(90);
  shooterServo.write(90);
  delay(1000);
  
  Serial.println("\n=== All servo tests complete ===");
  Serial.println("Repeating in 5 seconds...\n");
  delay(5000);
}

/*
 * EXPECTED BEHAVIOR:
 * 
 * CLAW SERVO:
 * - 0° = Fully closed (gripping)
 * - 45° = Partially closed (holding)
 * - 90° = Fully open (released)
 * 
 * SHOOTER SERVO:
 * - 0° = Ready position (loaded)
 * - 90° = Reset position
 * - 180° = Shoot position (launched)
 * 
 * TROUBLESHOOTING:
 * 
 * - Servo not moving:
 *   → Check power supply (needs 5V)
 *   → Verify PWM pin connection
 *   → Test with different servo
 * 
 * - Servo jittering:
 *   → Add 100µF capacitor across servo power
 *   → Ensure solid connections
 *   → Reduce mechanical load
 * 
 * - Servo moving to wrong angles:
 *   → Some servos are 180°, others 270°
 *   → Adjust angle ranges accordingly
 *   → Calibrate mechanical stops
 */
