/*
 * Color Sensor Test Sketch
 * Tests color detection using calibrated values from config.h
 * Point sensor at different colored surfaces to verify detection
 */

// Pin definitions
#define COLOR_S0   8
#define COLOR_S1   13
#define COLOR_S2   A2
#define COLOR_S3   A3
#define COLOR_OUT  A4

// Test with example thresholds (update after calibration)
#define BLACK_THRESHOLD_R  50
#define GREEN_THRESHOLD_G  150
#define RED_THRESHOLD_R    180
#define BLUE_THRESHOLD_B   180

void setup() {
  Serial.begin(9600);
  Serial.println("=== Color Sensor Test ===");
  Serial.println("Point sensor at colored surfaces");
  Serial.println();
  
  pinMode(COLOR_S0, OUTPUT);
  pinMode(COLOR_S1, OUTPUT);
  pinMode(COLOR_S2, OUTPUT);
  pinMode(COLOR_S3, OUTPUT);
  pinMode(COLOR_OUT, INPUT);
  
  // Set frequency scaling to 20%
  digitalWrite(COLOR_S0, HIGH);
  digitalWrite(COLOR_S1, LOW);
  
  delay(2000);
  Serial.println("Reading colors...\n");
}

void loop() {
  // Read RGB values
  int red = readRed();
  int green = readGreen();
  int blue = readBlue();
  
  // Print RGB values
  Serial.print("RGB: (");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.print(blue);
  Serial.print(")\t");
  
  // Detect and print color
  String color = detectColor(red, green, blue);
  Serial.print("Color: ");
  Serial.print(color);
  
  // Add color bar for visualization
  Serial.print("\t[");
  printColorBar(color);
  Serial.println("]");
  
  delay(300);
}

int readRed() {
  digitalWrite(COLOR_S2, LOW);
  digitalWrite(COLOR_S3, LOW);
  delay(10);
  return pulseIn(COLOR_OUT, LOW);
}

int readGreen() {
  digitalWrite(COLOR_S2, HIGH);
  digitalWrite(COLOR_S3, HIGH);
  delay(10);
  return pulseIn(COLOR_OUT, LOW);
}

int readBlue() {
  digitalWrite(COLOR_S2, LOW);
  digitalWrite(COLOR_S3, HIGH);
  delay(10);
  return pulseIn(COLOR_OUT, LOW);
}

String detectColor(int r, int g, int b) {
  // Black - all low values
  if (r < BLACK_THRESHOLD_R && g < BLACK_THRESHOLD_R && b < BLACK_THRESHOLD_R) {
    return "BLACK";
  }
  
  // White - all high values
  if (r > 200 && g > 200 && b > 200) {
    return "WHITE";
  }
  
  // Red - high red, low others
  if (r > RED_THRESHOLD_R && g < 100 && b < 100) {
    return "RED";
  }
  
  // Green - high green
  if (g > GREEN_THRESHOLD_G && r < 120) {
    return "GREEN";
  }
  
  // Blue - high blue
  if (b > BLUE_THRESHOLD_B && r < 100 && g < 120) {
    return "BLUE";
  }
  
  return "UNKNOWN";
}

void printColorBar(String color) {
  if (color == "BLACK") {
    Serial.print("████████");  // Full block
  } else if (color == "RED") {
    Serial.print("████░░░░");  // Red indicator
  } else if (color == "GREEN") {
    Serial.print("░░██████");  // Green indicator
  } else if (color == "BLUE") {
    Serial.print("░░░░████");  // Blue indicator
  } else if (color == "WHITE") {
    Serial.print("        ");  // Empty
  } else {
    Serial.print("????????");  // Unknown
  }
}

/*
 * TEST PROCEDURE:
 * 
 * 1. Point sensor at BLACK center:
 *    Expected: "BLACK" detection
 *    RGB should all be < 50
 * 
 * 2. Point sensor at GREEN ring:
 *    Expected: "GREEN" detection
 *    G value should be highest
 * 
 * 3. Point sensor at RED ring:
 *    Expected: "RED" detection
 *    R value should be highest
 * 
 * 4. Point sensor at BLUE ring:
 *    Expected: "BLUE" detection
 *    B value should be highest
 * 
 * 5. Point sensor at WHITE background:
 *    Expected: "WHITE" detection
 *    All RGB should be > 200
 * 
 * If detection is incorrect:
 * - Re-run color_sensor_calibration.ino
 * - Update threshold values in config.h
 * - Ensure consistent lighting conditions
 * - Check sensor distance (optimal: 5-10mm)
 */
