/*
 * Ultrasonic Sensor Test Sketch
 * Tests distance measurement and obstacle detection
 */

#define ULTRASONIC_TRIG  12
#define ULTRASONIC_ECHO  11
#define OBSTACLE_THRESHOLD 20  // cm

void setup() {
  Serial.begin(9600);
  Serial.println("=== Ultrasonic Sensor Test ===");
  Serial.println("Move objects in front of sensor\n");
  
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  
  delay(2000);
}

void loop() {
  float distance = measureDistance();
  
  Serial.print("Distance: ");
  
  if (distance >= 200 || distance == 0) {
    Serial.print("OUT OF RANGE");
  } else {
    Serial.print(distance);
    Serial.print(" cm\t");
    
    // Visual bar
    printDistanceBar(distance);
    Serial.print("\t");
    
    // Obstacle warning
    if (distance < OBSTACLE_THRESHOLD) {
      Serial.print("[OBSTACLE DETECTED!]");
    } else {
      Serial.print("[Clear path]");
    }
  }
  
  Serial.println();
  delay(200);
}

float measureDistance() {
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 30000);
  float distance = duration * 0.034 / 2;
  
  if (distance > 200 || distance == 0) {
    return 200;
  }
  
  return distance;
}

void printDistanceBar(float distance) {
  int bars = map(constrain(distance, 0, 100), 0, 100, 0, 20);
  
  Serial.print("|");
  for (int i = 0; i < 20; i++) {
    if (i < bars) {
      Serial.print("=");
    } else {
      Serial.print(" ");
    }
  }
  Serial.print("|");
}

/*
 * TEST PROCEDURE:
 * 
 * 1. No object in front:
 *    Expected: "OUT OF RANGE" or large distance (>100cm)
 * 
 * 2. Place hand at ~50cm:
 *    Expected: Reading around 50cm, "[Clear path]"
 * 
 * 3. Move hand closer to ~15cm:
 *    Expected: Reading around 15cm, "[OBSTACLE DETECTED!]"
 * 
 * 4. Move hand very close (<5cm):
 *    Expected: May show erratic or "OUT OF RANGE"
 * 
 * TROUBLESHOOTING:
 * 
 * - Always shows "OUT OF RANGE":
 *   → Check TRIG/ECHO wiring
 *   → Verify 5V power
 *   → Object too close (<2cm) or too far (>200cm)
 * 
 * - Readings fluctuate wildly:
 *   → Add capacitor across power pins
 *   → Check for motor interference
 *   → Ensure sensor is stable
 * 
 * - Readings consistently off:
 *   → Recalculate speed of sound factor
 *   → Check temperature (affects sound speed)
 *   → Verify object is perpendicular
 */
