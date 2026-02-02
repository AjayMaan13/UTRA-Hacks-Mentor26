/*
 * IR Sensor Test Sketch
 * Tests IR sensors for line detection
 */

#define IR_LEFT   A0
#define IR_RIGHT  A1
#define IR_THRESHOLD  500  // Update after calibration

void setup() {
  Serial.begin(9600);
  Serial.println("=== IR Sensor Test ===");
  Serial.println("Place sensors over black line to test detection\n");
  
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  
  delay(2000);
}

void loop() {
  int leftValue = analogRead(IR_LEFT);
  int rightValue = analogRead(IR_RIGHT);
  
  // Print raw values
  Serial.print("LEFT: ");
  Serial.print(leftValue);
  Serial.print("\t");
  
  // Show left sensor status
  if (leftValue < IR_THRESHOLD) {
    Serial.print("[LINE]");
  } else {
    Serial.print("[----]");
  }
  
  Serial.print("\t|\t");
  
  Serial.print("RIGHT: ");
  Serial.print(rightValue);
  Serial.print("\t");
  
  // Show right sensor status
  if (rightValue < IR_THRESHOLD) {
    Serial.print("[LINE]");
  } else {
    Serial.print("[----]");
  }
  
  Serial.print("\t|\t");
  
  // Determine line position
  String position = getLinePosition(leftValue, rightValue);
  Serial.println(position);
  
  delay(200);
}

String getLinePosition(int left, int right) {
  bool leftOnLine = (left < IR_THRESHOLD);
  bool rightOnLine = (right < IR_THRESHOLD);
  
  if (leftOnLine && rightOnLine) {
    return "CENTERED ===";
  } else if (leftOnLine && !rightOnLine) {
    return "LEFT     <<<";
  } else if (!leftOnLine && rightOnLine) {
    return "RIGHT    >>>";
  } else {
    return "NO LINE  ???";
  }
}

/*
 * EXPECTED BEHAVIOR:
 * 
 * Robot over BLACK line:
 * - LEFT: [LINE] RIGHT: [LINE] CENTERED
 * 
 * Line under left sensor only:
 * - LEFT: [LINE] RIGHT: [----] LEFT
 * 
 * Line under right sensor only:
 * - LEFT: [----] RIGHT: [LINE] RIGHT
 * 
 * Robot over WHITE surface:
 * - LEFT: [----] RIGHT: [----] NO LINE
 * 
 * TROUBLESHOOTING:
 * 
 * - Both always show [LINE]:
 *   → Threshold too high, decrease IR_THRESHOLD
 *   → Check ambient light (too dark)
 * 
 * - Both always show [----]:
 *   → Threshold too low, increase IR_THRESHOLD
 *   → Check sensor height (should be 5-15mm)
 * 
 * - Inconsistent readings:
 *   → Sensor too far from surface
 *   → Loose connections
 *   → Re-calibrate sensors
 */
