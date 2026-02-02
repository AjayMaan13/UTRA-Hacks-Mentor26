# ⚡ Quick Start Guide

Get your Winter Olympics Robot running in **15 minutes**!

---

## Prerequisites

✅ Arduino IDE 2.0+ installed  
✅ Arduino Uno board  
✅ USB-C cable  
✅ All components from kit assembled  

---

## Step 1: Hardware Assembly (5 minutes)

### 1.1 Mount Motors to Base
```
1. Attach motor holders to laser-cut base with screws
2. Insert DC motors into holders
3. Connect wheels to motor shafts
```

### 1.2 Mount Arduino & Breadboard
```
1. Use double-sided tape or standoffs
2. Place Arduino near center for balance
3. Breadboard next to Arduino
```

### 1.3 Connect Motor Driver
```
L298N Connections:
- OUT1/2 → Left Motor (red/black)
- OUT3/4 → Right Motor (red/black)
- +12V → Battery Pack (+)
- GND → Battery Pack (-) + Arduino GND ⚠️ COMMON GROUND!
```

### 1.4 Wire Sensors
```
Front: Ultrasonic sensor (facing forward)
Bottom Front: 2× IR sensors (8cm apart, facing down)
Bottom Center: Color sensor (1-2cm from ground)
```

---

## Step 2: Software Setup (3 minutes)

### 2.1 Download Code
```bash
git clone https://github.com/yourusername/winter-olympics-robot.git
cd winter-olympics-robot
```

**OR** Download ZIP from GitHub → Extract → Open in Arduino IDE

### 2.2 Install Libraries
```
Arduino IDE → Sketch → Include Library → Manage Libraries

Search & Install:
- "Servo" (built-in, should already be installed)
```

### 2.3 Select Board & Port
```
Tools → Board → Arduino AVR Boards → Arduino Uno
Tools → Port → (Select your Arduino, e.g., COM3 or /dev/ttyACM0)
```

---

## Step 3: Calibration (5 minutes)

### 3.1 Run Calibration Sketch
```
File → Open → winter-olympics-robot/calibration/sensor_calibration.ino
Click Upload (→)
```

### 3.2 Open Serial Monitor
```
Tools → Serial Monitor
Or: Ctrl+Shift+M (Windows/Linux) or Cmd+Shift+M (Mac)
Set baud rate: 9600
```

### 3.3 Follow Prompts
```
1. Choose option "2" (Calibrate Color Sensor)
2. Place sensor over RED → Press Enter → Record values
3. Repeat for GREEN, BLUE, BLACK, WHITE
4. Choose option "4" to print calibration code
5. Copy values to main/main.ino (lines 40-45)
```

### 3.4 Quick Test
```
Choose option "5" (Test All Sensors)
Check readings are sensible:
- IR: Should toggle 0/1 over black line
- Distance: Should show distance in cm
- Color: Should detect surface colors
```

---

## Step 4: Upload Main Code (2 minutes)

### 4.1 Open Main Sketch
```
File → Open → winter-olympics-robot/main/main.ino
```

### 4.2 Verify Code
```
Click Verify (✓)
Wait for "Done compiling"
Check: No errors (warnings are OK)
```

### 4.3 Upload to Robot
```
Click Upload (→)
Wait for "Done uploading"
Arduino LED should blink during upload
```

---

## Step 5: First Run! (Let's Go! 🚀)

### 5.1 Power Check
```
☑ Batteries installed (check voltage > 7V)
☑ All connections secure
☑ Arduino LED is on
☑ Motor driver LED is on
☑ No loose wires near wheels
```

### 5.2 Position Robot
```
1. Place on black starting line
2. Ensure gripper is open
3. Box positioned in front of robot
```

### 5.3 Start the Challenge
```
Press Arduino RESET button
Robot waits 3 seconds...
Then begins autonomous operation!
```

### 5.4 Monitor via Serial
```
Open Serial Monitor to see:
- Current state
- Sensor readings
- Distance measurements
- Color detections
- Debug info
```

---

## Troubleshooting

### Robot doesn't move
```
✓ Check: Battery voltage
✓ Check: Motor driver connections
✓ Check: ENA/ENB jumpers removed from L298N
✓ Test: Upload example sketch: File → Examples → Basics → Blink
```

### Motors spin wrong direction
```
✓ Fix: Swap motor wires at L298N terminal
✓ OR: Change code (line 200-230, swap HIGH/LOW)
```

### Sensors not working
```
✓ Check: 5V and GND connected to sensors
✓ Check: Signal pins match code pin definitions
✓ Test: Run calibration sketch to verify readings
```

### Arduino resets randomly
```
✓ Cause: Voltage drop from motors
✓ Fix: Use separate battery for motors
✓ Fix: Add capacitor to Arduino VIN (220µF)
```

### Can't upload code
```
✓ Check: USB cable connected
✓ Check: Correct board selected (Arduino Uno)
✓ Check: Correct port selected
✓ Try: Different USB cable
✓ Try: Press reset button just before upload
```

---

## Quick Test Commands

### Test Individual Functions

**Open Serial Monitor and send:**

```
f - Move forward 2 seconds
b - Move backward 2 seconds
l - Turn left 1 second
r - Turn right 1 second
s - Stop all motors
g - Test gripper (open/close)
c - Read color sensor
d - Toggle debug mode
```

**Example:**
```
1. Type 'f' and press Enter → Robot moves forward
2. Type 's' and press Enter → Robot stops
3. Type 'c' and press Enter → Shows color reading
```

---

## Performance Tips

### Make Robot Faster
```cpp
// In main.ino, change:
#define BASE_SPEED 160    // Try: 180, 200 (careful!)
```

### Make Line Following Smoother
```cpp
// Tune PID constants:
const float Kp = 0.8;     // Try: 0.5 to 1.5
const float Kd = 0.3;     // Try: 0.1 to 0.5
```

### Improve Color Detection
```cpp
// Increase samples:
for (int i = 0; i < 10; i++) {  // Try: 15 or 20
```

---

## Next Steps

### Once Basic Operation Works:

1. **Practice the Course**
   - Run through obstacles
   - Test ramp climbing
   - Verify ball shooting

2. **Optimize Performance**
   - Tune motor speeds
   - Adjust PID constants
   - Calibrate gripper timing

3. **Add Features**
   - Encoder support
   - Bluetooth control
   - LCD display
   - LED indicators

4. **Document Your Journey**
   - Take photos/videos
   - Write about challenges
   - Share on social media

---

## Useful Serial Commands

### During Development:

```cpp
// Add to your code for debugging:
Serial.print("Motor Speed: ");
Serial.println(speed);

Serial.print("State: ");
Serial.println(currentState);

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
```

---

## Competition Checklist

Before the official run:

- [ ] Fresh batteries installed
- [ ] All sensors calibrated
- [ ] Test run completed successfully
- [ ] Motor speeds tuned
- [ ] Gripper tested with actual box
- [ ] Ball launching mechanism tested
- [ ] Code backed up
- [ ] Team roles assigned

---

## Help & Support

**Need Help?**
- 📖 Read: `docs/WIRING.md` for connection details
- 🐛 Check: `docs/TROUBLESHOOTING.md` for common issues
- 💬 Ask: Open GitHub Issue with your question
- 📧 Email: your.email@example.com

**Resources:**
- Arduino Reference: https://www.arduino.cc/reference/en/
- Project Documentation: `docs/` folder
- Example Code: `examples/` folder
- Calibration Tools: `calibration/` folder

---

## Success! 🎉

**If your robot:**
- ✅ Picks up the box
- ✅ Navigates the path
- ✅ Detects colors correctly
- ✅ Avoids obstacles

**You're ready to compete!**

Now go practice and **win that hackathon! 🏆**

---

**Pro Tip:** Start simple! Get basic movement working first, then add complexity. Don't try to do everything at once.

**Remember:** Every great robot project started with a blinking LED. You've got this! 💪
