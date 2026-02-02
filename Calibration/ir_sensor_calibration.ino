/*
 * IR Sensor Calibration Sketch
 * Use this to determine threshold values for line detection
 * 
 * Instructions:
 * 1. Upload this sketch
 * 2. Open Serial Monitor (9600 baud)
 * 3. Place LEFT sensor over WHITE surface → note "White" value
 * 4. Place LEFT sensor over BLACK line → note "Black" value
 * 5. Repeat for RIGHT sensor
 * 6. Calculate threshold: THRESHOLD = (White + Black) / 2
 * 7. Update IR_THRESHOLD in config.h
 */

// Pin definitions
#define IR_LEFT   A0
#define IR_RIGHT  A1

void setup() {
  Serial.begin(9600);
  Serial.println("=== IR Sensor Calibration ===");
  Serial.println("Place sensors over different surfaces");
  Serial.println("Record the values for calibration");
  Serial.println();
  
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  
  delay(2000);
}

void loop() {
  // Read both IR sensors
  int leftValue = analogRead(IR_LEFT);
  int rightValue = analogRead(IR_RIGHT);
  
  // Print values
  Serial.print("LEFT: ");
  Serial.print(leftValue);
  Serial.print("\t");
  
  // Indicate if on line (using example threshold of 500)
  if (leftValue < 500) {
    Serial.print("(BLACK LINE)");
  } else {
    Serial.print("(WHITE)     ");
  }
  
  Serial.print("\t|\t");
  
  Serial.print("RIGHT: ");
  Serial.print(rightValue);
  Serial.print("\t");
  
  if (rightValue < 500) {
    Serial.print("(BLACK LINE)");
  } else {
    Serial.print("(WHITE)     ");
  }
  
  Serial.println();
  
  delay(200);
}

/*
 * CALIBRATION NOTES:
 * 
 * Typical values:
 * - White surface: 700-1000
 * - Black line: 100-300
 * - Optimal threshold: ~500 (midpoint)
 * 
 * If your values are different:
 * 1. Find your WHITE reading (max value)
 * 2. Find your BLACK reading (min value)
 * 3. Set threshold = (WHITE + BLACK) / 2
 * 
 * Example:
 * - White = 850
 * - Black = 200
 * - Threshold = (850 + 200) / 2 = 525
 * 
 * Update config.h:
 * #define IR_THRESHOLD 525
 */
