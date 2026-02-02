# Quick Start Guide 🚀

This guide will help you get your Winter Olympics robot up and running quickly.

## Prerequisites

### Software Requirements
- **Arduino IDE** (version 1.8.x or 2.x)
  - Download from: https://www.arduino.cc/en/software
- **USB-C cable** for Arduino connection

### Hardware Requirements
- All components from the hacker kit (see [README.md](../README.md#hardware-components))
- Fully assembled robot chassis
- Charged 9V batteries

## Step-by-Step Setup

### 1. Install Arduino IDE

1. Download and install Arduino IDE from the official website
2. Launch Arduino IDE
3. Go to `Tools → Board → Arduino AVR Boards → Arduino Uno`
4. Select your COM port: `Tools → Port → COM# (Arduino Uno)`

### 2. Clone the Repository

```bash
git clone https://github.com/yourusername/winter-olympics-robot.git
cd winter-olympics-robot
```

### 3. Calibrate Sensors (IMPORTANT!)

**Run calibrations in this order:**

#### A. Motor Calibration
```arduino
File → Open → calibration/motor_calibration.ino
```
- Adjust `MOTOR_SPEED` constants until wheels spin at similar speeds
- Note down calibration values

#### B. IR Sensor Calibration
```arduino
File → Open → calibration/ir_sensor_calibration.ino
```
- Place sensors over white and black surfaces
- Record threshold values in Serial Monitor
- Update `IR_THRESHOLD` in `config.h`

#### C. Color Sensor Calibration
```arduino
File → Open → calibration/color_sensor_calibration.ino
```
- Point sensor at each colored surface (black, green, red, blue)
- Record RGB values
- Update color thresholds in `config.h`

#### D. Ultrasonic Sensor Calibration
```arduino
File → Open → calibration/ultrasonic_calibration.ino
```
- Measure distances to various objects
- Verify accuracy
- Update `OBSTACLE_DISTANCE` threshold if needed

### 4. Run Component Tests

Test each component individually before running the main program:

```arduino
// Test motors
File → Open → tests/test_motors.ino

// Test servos
File → Open → tests/test_servos.ino

// Test color sensor
File → Open → tests/test_color_sensor.ino

// Test IR sensors
File → Open → tests/test_ir_sensors.ino

// Test ultrasonic sensor
File → Open → tests/test_ultrasonic.ino
```

✅ **Only proceed if all tests pass!**

### 5. Configure Main Program

1. Open `src/config.h`
2. Update calibration values from step 3:
   ```cpp
   // Example values (replace with your calibrated values)
   #define IR_THRESHOLD 500
   #define MOTOR_LEFT_SPEED 200
   #define MOTOR_RIGHT_SPEED 195
   #define OBSTACLE_DISTANCE 20
   ```

### 6. Upload Main Program

```arduino
File → Open → src/main.ino
Sketch → Upload
```

Wait for "Done uploading" message.

### 7. Power On and Test

1. **Disconnect USB** (robot should run on battery power)
2. Install 9V batteries in holders
3. Place robot at starting position
4. Turn on power switch
5. Robot should begin autonomous operation!

## 🔧 Advanced Tuning

### PID Tuning for Line Following
```cpp
// In config.h
#define KP 1.0  // Proportional gain
#define KI 0.0  // Integral gain  
#define KD 0.5  // Derivative gain
```

### Speed Optimization
```cpp
// Adjust for your surface
#define BASE_SPEED 180
#define TURN_SPEED 120
#define SLOW_SPEED 100
```

### Sensor Update Rates
```cpp
#define SENSOR_DELAY 50  // ms between sensor reads
#define CONTROL_LOOP_DELAY 10  // ms per control loop
```

## 📚 Additional Resources

- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [L298N Motor Driver Guide](https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/)
- [TCS3200 Color Sensor Tutorial](https://lastminuteengineers.com/tcs3200-color-sensor-arduino-tutorial/)
- [HC-SR04 Ultrasonic Tutorial](https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/)

## 💡 Pro Tips

1. **Calibrate on the actual competition surface** - different materials affect sensors
2. **Keep code modular** - easier to debug and modify
3. **Add Serial.print() statements** - helps with debugging
4. **Test incrementally** - one feature at a time
5. **Have backup batteries** - fresh batteries = better performance
6. **Practice, practice, practice** - autonomous robots need tuning!

## 🆘 Getting Help

- Check the [main README](../README.md) for project overview
- Review calibration files in `/calibration` folder
- Examine test files in `/tests` folder
- Open an issue on GitHub for bugs or questions

---

**Ready to compete?** Good luck! 🏆

Remember: The best robot is the one that completes the course reliably, not necessarily the fastest one!
