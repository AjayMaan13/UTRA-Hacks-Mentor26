/*
 * Ultrasonic Sensor Calibration Sketch
 * Use this to verify distance measurements and set obstacle thresholds
 * 
 * Instructions:
 * 1. Upload this sketch
 * 2. Open Serial Monitor (9600 baud)
 * 3. Place objects at known distances (use ruler)
 * 4. Compare measured vs. actual distances
 * 5. Determine appropriate OBSTACLE_DISTANCE threshold
 * 6. Update config.h with calibrated values
 */

// Pin definitions
#define ULTRASONIC_TRIG  12
#define ULTRASONIC_ECHO  11

#define MAX_DISTANCE     200  // Maximum reliable distance in cm

void setup() {
  Serial.begin(9600);
  Serial.println("=== Ultrasonic Sensor Calibration ===");
  Serial.println("Measuring distances continuously");
  Serial.println("Place objects at known distances to verify accuracy");
  Serial.println();
  
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  pinMode(ULTRASONIC_ECHO, INPUT);
  
  delay(2000);
  Serial.println("Starting measurements...\n");
}

void loop() {
  // Measure distance
  float distance = measureDistance();
  
  // Print distance with status
  Serial.print("Distance: ");
  
  if (distance >= MAX_DISTANCE || distance == 0) {
    Serial.print("OUT OF RANGE");
  } else {
    Serial.print(distance);
    Serial.print(" cm");
    
    // Add interpretation
    if (distance < 10) {
      Serial.print("\t[VERY CLOSE - Ramp detection range]");
    } else if (distance < 20) {
      Serial.print("\t[CLOSE - Obstacle detection range]");
    } else if (distance < 50) {
      Serial.print("\t[MEDIUM - Safe navigation range]");
    } else {
      Serial.print("\t[FAR - Clear path]");
    }
  }
  
  Serial.println();
  
  delay(250);
}

float measureDistance() {
  // Send trigger pulse
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);
  
  // Measure echo pulse duration (timeout after 30ms)
  long duration = pulseIn(ULTRASONIC_ECHO, HIGH, 30000);
  
  // Calculate distance in cm
  // Speed of sound = 343 m/s = 0.0343 cm/µs
  // Distance = (duration × 0.0343) / 2
  float distance = duration * 0.034 / 2;
  
  // Return 0 if timeout or out of range
  if (distance > MAX_DISTANCE || distance == 0) {
    return MAX_DISTANCE;
  }
  
  return distance;
}

/*
 * CALIBRATION GUIDE:
 * 
 * 1. VERIFY ACCURACY:
 *    - Place ruler perpendicular to sensor
 *    - Position object at 10cm, 20cm, 30cm, etc.
 *    - Compare Serial Monitor reading with ruler
 *    - Typical accuracy: ±2cm
 * 
 * 2. OBSTACLE DETECTION:
 *    - Test at various distances
 *    - Determine safe stopping distance
 *    - Recommended: 15-25cm for obstacles
 *    - Update OBSTACLE_DISTANCE in config.h
 * 
 * 3. RAMP DETECTION:
 *    - Measure distance to ramp base
 *    - Typical: 8-12cm
 *    - Update RAMP_DISTANCE in config.h
 * 
 * 4. LIMITATIONS:
 *    - Soft/angled surfaces may absorb sound
 *    - Very small objects might not be detected
 *    - Maximum reliable range: ~200cm
 *    - Minimum reliable range: ~2cm
 * 
 * RECOMMENDED CONFIG.H VALUES:
 * 
 * #define OBSTACLE_DISTANCE  20   // Stop before hitting obstacles
 * #define RAMP_DISTANCE      10   // Detect ramp approach
 * #define MAX_DISTANCE       200  // Sensor limit
 * 
 * TROUBLESHOOTING:
 * 
 * - Reading stuck at 0:
 *   → Check wiring (TRIG and ECHO pins)
 *   → Ensure 5V power supply
 * 
 * - Erratic readings:
 *   → Add 10µF capacitor across VCC and GND
 *   → Ensure sensor is firmly mounted
 *   → Check for electrical noise from motors
 * 
 * - Readings always MAX_DISTANCE:
 *   → Check if object is within 2cm-200cm range
 *   → Ensure object is perpendicular to sensor
 *   → Test with larger, flat surface
 */
