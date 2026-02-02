# 🏂 Winter Olympics Biathlon Robot

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> An autonomous robot inspired by the Winter Olympics Biathlon event, capable of navigating obstacle courses, detecting colored paths, and launching projectiles with precision.

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software Architecture](#software-architecture)
- [Installation](#installation)
- [Usage](#usage)
- [Challenge Sections](#challenge-sections)
- [Results](#results)
- [Future Improvements](#future-improvements)
- [License](#license)

## 🎯 Overview

This project was developed for the **Seneca UTRA Winter Olympics Hackathon**, where teams designed autonomous robots to complete a biathlon-inspired course. The robot demonstrates the three core principles of robotics:

1. **Perception** - Using IR sensors, color sensors, and ultrasonic sensors to understand the environment
2. **Reasoning** - State machine-based decision making and path planning
3. **Actuation** - Precise motor control and servo-based mechanisms

### Competition Results
- **Track Score**: 45/63 points
- **Presentation Score**: 35/40 points
- **Total Score**: 80/103 points
- **Highlights**: Successfully completed both sections, curved ramp navigation, accurate ball shooting

## ✨ Features

### Core Capabilities
- ✅ **Autonomous Navigation** - Line following using IR sensors with PID control
- ✅ **Color Detection** - TCS3200 color sensor for path differentiation (Red/Green paths)
- ✅ **Object Manipulation** - Servo-controlled gripper for box pickup and placement
- ✅ **Obstacle Avoidance** - Ultrasonic sensor-based collision prevention
- ✅ **Ball Launching** - Catapult mechanism for target shooting
- ✅ **Ramp Climbing** - Torque-optimized motor control for incline navigation
- ✅ **State Machine Logic** - Modular, maintainable code architecture

### Advanced Features
- 🎯 Non-blocking timing using `millis()`
- 🎯 Sensor data filtering and calibration
- 🎯 Dynamic speed adjustment based on terrain
- 🎯 Fail-safe timeout mechanisms
- 🎯 Serial debugging interface
- 🎯 Modular code structure for easy modifications

## 🔧 Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| Arduino Uno (USB-C) | 1 | Main microcontroller |
| L298N Motor Driver | 1 | DC motor control |
| DC Gear Motors | 2 | Drivetrain |
| Yellow Wheels | 4 | Mobility |
| SG90 Servo Motors | 2 | Gripper & launcher |
| HC-SR04 Ultrasonic Sensor | 1 | Distance measurement |
| TCS3200 Color Sensor | 1 | Path detection |
| TCRT5000 IR Sensors | 2 | Line following |
| 9V Batteries | 4 | Power supply |
| Breadboard | 1 | Circuit prototyping |
| Laser-Cut Acrylic Base | 1 | Robot chassis |
| Jumper Wires (M-M, M-F, F-F) | 60 | Connections |

### Pin Configuration

```
MOTORS:
- Motor Left:  IN1=2, IN2=3, ENA=9 (PWM)
- Motor Right: IN3=4, IN4=5, ENB=10 (PWM)

SENSORS:
- IR Left:     Pin 11
- IR Right:    Pin 12
- Ultrasonic:  Trig=7, Echo=8
- Color:       S0=A0, S1=A1, S2=A2, S3=A3, OUT=A4

ACTUATORS:
- Gripper Servo:   Pin 6 (PWM)
- Launcher Servo:  Pin 13
```

## 🏗️ Software Architecture

### State Machine Design

```
IDLE → PICKUP_BOX → NAVIGATE_TO_SPLIT → DETECT_PATH → 
  ↓
  ├─→ GREEN_PATH → CLIMB_RAMP → NAVIGATE_TARGET → SHOOT_BALL → RETURN
  │
  └─→ RED_PATH → OBSTACLE_COURSE → RETURN → COMPLETE
```

### Key Algorithms

**1. PID Line Following**
```cpp
error = leftSensor - rightSensor
correction = (error * Kp) + (derivative * Kd)
leftMotorSpeed = baseSpeed - correction
rightMotorSpeed = baseSpeed + correction
```

**2. Color Detection**
```cpp
Read RGB values from TCS3200
Compare against calibrated thresholds
Determine: RED, GREEN, BLUE, BLACK, or WHITE
```

**3. Obstacle Avoidance**
```cpp
if (distance < 15cm) {
  STOP → BACKUP → TURN_RIGHT → CONTINUE
}
```

## 📦 Installation

### Prerequisites
- Arduino IDE 2.0+
- USB-C cable
- Arduino Uno board

### Setup Steps

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/winter-olympics-robot.git
cd winter-olympics-robot
```

2. **Install required libraries**
   - Open Arduino IDE
   - Go to `Sketch → Include Library → Manage Libraries`
   - Install:
     - `Servo` (built-in)
     - `NewPing` (for ultrasonic sensor)

3. **Configure Arduino IDE**
   - Tools → Board → Arduino AVR Boards → Arduino Uno
   - Tools → Port → Select your Arduino port
   - Tools → Programmer → AVRISP mkII

4. **Upload the code**
   - Open `main/main.ino`
   - Click Upload button (→)
   - Wait for "Upload Complete"

5. **Calibration** (Important!)
   - Open Serial Monitor (Ctrl+Shift+M)
   - Follow on-screen calibration prompts
   - Place robot over each colored surface
   - Record threshold values

## 🚀 Usage

### Quick Start

1. **Power on the robot**
   - Ensure all 4 batteries are connected
   - Check LED indicator on Arduino

2. **Position at START**
   - Place robot at the black starting line
   - Ensure gripper is open

3. **Run the program**
   - Press the Arduino reset button
   - Robot will begin after 3-second countdown

### Serial Commands (Debugging)

| Command | Action |
|---------|--------|
| `f` | Move forward |
| `b` | Move backward |
| `l` | Turn left |
| `r` | Turn right |
| `s` | Stop motors |
| `g` | Test gripper (open/close) |
| `c` | Calibrate color sensor |
| `d` | Debug mode toggle |

### Calibration Process

Run `calibration/sensor_calibration.ino` first:

```cpp
// 1. Color Sensor Calibration
Place over BLACK → record values
Place over RED → record values
Place over GREEN → record values
Place over BLUE → record values
Place over WHITE → record values

// 2. IR Sensor Calibration
Place over black line → record LOW value
Place over white surface → record HIGH value

// 3. Motor Speed Calibration
Drive straight for 2 meters
Adjust LEFT_MOTOR_OFFSET in config.h
```

## 🏁 Challenge Sections

### Section 1: Target Shooting (25 points)

**Tasks:**
1. Pick up box from start area (5 pts)
2. Navigate to green path (1 pt)
3. Climb ramp - curved ramp chosen (4 pts)
4. Navigate colored target rings to center (15 pts)
   - Blue ring → Red ring → Green ring → Black center
5. Shoot ball into scoring zone (5 pts - landed in blue zone)

**Strategy:**
- Used color sensor to detect concentric rings
- Implemented spiral navigation algorithm
- Servo catapult with 120° release angle
- Ball velocity: ~2 m/s

**Code:** `src/section1_target_shooting/`

### Section 2: Obstacle Course (20 points)

**Tasks:**
1. Pick up second box (5 pts)
2. Navigate winding red path (5 pts)
3. Avoid black obstacles using ultrasonic (5 pts)
4. Complete course under 60 seconds (5 pts)

**Strategy:**
- PID line following with Kp=0.8, Kd=0.3
- Ultrasonic range: 5-30cm detection
- Dynamic speed: slow on curves, fast on straights
- Completion time: 58 seconds

**Code:** `src/section2_obstacle_course/`

## 📊 Results

### Performance Metrics

| Metric | Target | Achieved | Score |
|--------|--------|----------|-------|
| Box Pickup | Clean | Partial drag | 3/5 |
| Path Detection | Both | Both | 1/1 |
| Ramp (Curved) | Complete | Complete | 4/4 |
| Target Navigation | Center | Center | 15/15 |
| Ball Shooting | Blue zone | Blue zone | 5/5 |
| Obstacle Avoidance | 0 touches | 1 touch | 4/5 |
| Course Time | <60s | 58s | 5/5 |

**Total Track Score: 45/63**

### Presentation Score: 35/40

- Build Quality: 7/10
- Visual Presentation: 3/3
- Explanation Clarity: 8/8
- Technical Complexity: 10/10
- Q&A: 5/5
- Problem-Solving: 2/2

## 🔮 Future Improvements

### Short-term (v2.0)
- [ ] Implement encoder-based position tracking
- [ ] Add Bluetooth module for wireless debugging
- [ ] Improve gripper mechanism with force sensors
- [ ] Optimize PID constants with auto-tuning
- [ ] Add LCD display for status indication

### Long-term (v3.0)
- [ ] Upgrade to Arduino Mega for more I/O
- [ ] Implement computer vision with Raspberry Pi
- [ ] Add gyroscope/accelerometer for orientation
- [ ] Develop machine learning path prediction
- [ ] Create web dashboard for real-time monitoring

## 🎓 Lessons Learned

### Technical Insights
1. **Sensor Calibration is Critical** - Spent 2 hours on color sensor calibration, saved 30 minutes debugging
2. **Non-blocking Code** - Using `millis()` instead of `delay()` improved responsiveness 10x
3. **Modular Design** - State machine made debugging individual sections much easier
4. **Power Management** - Fresh batteries essential; voltage drop caused erratic servo behavior

### Team Collaboration
- Parallel development: one person on hardware, one on software
- Version control prevented code conflicts
- Regular testing caught issues early

## 📸 Media

### Robot Photos
- [Front View](docs/images/robot_front.jpg)
- [Gripper Mechanism](docs/images/gripper.jpg)
- [Sensor Array](docs/images/sensors.jpg)
- [In Action](docs/images/action.gif)

### Demo Videos
- [Full Course Run](https://youtube.com/...)
- [Target Shooting](https://youtube.com/...)
- [Obstacle Course](https://youtube.com/...)

## 🤝 Contributing

This project is open for educational purposes. Feel free to:
- Fork the repository
- Submit pull requests with improvements
- Open issues for bugs or questions
- Use as reference for your own projects

## 📄 License

MIT License - see [LICENSE](LICENSE) file

## 🙏 Acknowledgments

- **Seneca UTRA** for organizing the hackathon
- **Mentors** for hardware support and debugging help
- **Teammates** for collaborative problem-solving
- **Arduino Community** for excellent documentation

## 📞 Contact

**Ajay** - Peer Mentor at Seneca UTRA  
- GitHub: [@yourusername](https://github.com/yourusername)
- LinkedIn: [Your LinkedIn](https://linkedin.com/in/yourprofile)
- Email: your.email@example.com

---

**⭐ If you found this project helpful, please give it a star!**

*Built with ❤️ during Seneca UTRA Winter Olympics Hackathon 2026*
