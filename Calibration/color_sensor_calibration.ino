/*
 * Color Sensor Calibration Sketch
 * Use this to calibrate RGB threshold values for each color
 * 
 * Instructions:
 * 1. Upload this sketch
 * 2. Open Serial Monitor (9600 baud)
 * 3. Point sensor at BLACK surface → record RGB values
 * 4. Point sensor at GREEN surface → record RGB values
 * 5. Point sensor at RED surface → record RGB values
 * 6. Point sensor at BLUE surface → record RGB values
 * 7. Point sensor at WHITE surface → record RGB values
 * 8. Update color threshold ranges in config.h
 */

// Pin definitions
#define COLOR_S0   8
#define COLOR_S1   13
#define COLOR_S2   A2
#define COLOR_S3   A3
#define COLOR_OUT  A4

void setup() {
  Serial.begin(9600);
  Serial.println("=== Color Sensor Calibration ===");
  Serial.println("Point sensor at each color and record RGB values");
  Serial.println("Format: R=xxx  G=xxx  B=xxx");
  Serial.println();
  
  // Initialize color sensor pins
  pinMode(COLOR_S0, OUTPUT);
  pinMode(COLOR_S1, OUTPUT);
  pinMode(COLOR_S2, OUTPUT);
  pinMode(COLOR_S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);
  
  // Set frequency scaling to 20%
  digitalWrite(COLOR_S0, HIGH);
  digitalWrite(COLOR_S1, LOW);
  
  delay(2000);
  Serial.println("Starting calibration...\n");
}

void loop() {
  // Read RGB values
  int red = readRed();
  int green = readGreen();
  int blue = readBlue();
  
  // Print values
  Serial.print("R=");
  Serial.print(red);
  Serial.print("\tG=");
  Serial.print(green);
  Serial.print("\tB=");
  Serial.print(blue);
  Serial.print("\t|\t");
  
  // Guess color (basic detection)
  String detectedColor = guessColor(red, green, blue);
  Serial.print("Detected: ");
  Serial.println(detectedColor);
  
  delay(500);
}

// Read red filtered value
int readRed() {
  digitalWrite(COLOR_S2, LOW);
  digitalWrite(COLOR_S3, LOW);
  delay(10);
  int frequency = pulseIn(COLOR_OUT, LOW);
  return frequency;
}

// Read green filtered value
int readGreen() {
  digitalWrite(COLOR_S2, HIGH);
  digitalWrite(COLOR_S3, HIGH);
  delay(10);
  int frequency = pulseIn(COLOR_OUT, LOW);
  return frequency;
}

// Read blue filtered value
int readBlue() {
  digitalWrite(COLOR_S2, LOW);
  digitalWrite(COLOR_S3, HIGH);
  delay(10);
  int frequency = pulseIn(COLOR_OUT, LOW);
  return frequency;
}

// Basic color detection (adjust thresholds based on your readings)
String guessColor(int r, int g, int b) {
  // These are example thresholds - adjust based on your calibration!
  
  // Black (low values for all)
  if (r < 50 && g < 50 && b < 50) {
    return "BLACK";
  }
  
  // White (high values for all)
  if (r > 200 && g > 200 && b > 200) {
    return "WHITE";
  }
  
  // Red (high red, low green and blue)
  if (r > 150 && g < 80 && b < 80) {
    return "RED";
  }
  
  // Green (high green, lower red and blue)
  if (g > 100 && r < 100 && b < 100) {
    return "GREEN";
  }
  
  // Blue (high blue, low red and green)
  if (b > 150 && r < 80 && g < 100) {
    return "BLUE";
  }
  
  return "UNKNOWN";
}

/*
 * CALIBRATION PROCEDURE:
 * 
 * 1. Point sensor at BLACK surface:
 *    Record: R=___ G=___ B=___
 *    These are your BLACK_x_MIN and BLACK_x_MAX values
 * 
 * 2. Point sensor at GREEN surface:
 *    Record: R=___ G=___ B=___
 *    These are your GREEN_x_MIN and GREEN_x_MAX values
 * 
 * 3. Point sensor at RED surface:
 *    Record: R=___ G=___ B=___
 *    These are your RED_x_MIN and RED_x_MAX values
 * 
 * 4. Point sensor at BLUE surface:
 *    Record: R=___ G=___ B=___
 *    These are your BLUE_x_MIN and BLUE_x_MAX values
 * 
 * 5. Point sensor at WHITE surface:
 *    Record: R=___ G=___ B=___
 *    These are your WHITE_x_MIN and WHITE_x_MAX values
 * 
 * TIPS:
 * - Take multiple readings for each color
 * - Use average values for more reliable detection
 * - Add ±10-20 margin for MIN/MAX ranges
 * - Test under same lighting as competition
 * - Avoid direct sunlight (causes interference)
 * 
 * Example calibration for RED:
 * Readings: R=180, G=45, B=50
 * Set in config.h:
 * #define RED_R_MIN 160  (180 - 20 margin)
 * #define RED_R_MAX 200  (180 + 20 margin)
 * #define RED_G_MIN 25   (45 - 20 margin)
 * #define RED_G_MAX 65   (45 + 20 margin)
 * #define RED_B_MIN 30   (50 - 20 margin)
 * #define RED_B_MAX 70   (50 + 20 margin)
 */
