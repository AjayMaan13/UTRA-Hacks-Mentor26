# Winter Olympics Biathlon Robot 🏂🎯

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![UTRA Hacks](https://img.shields.io/badge/UTRA_Hacks-2025-blue)](https://hackathon.utra.ca/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

An autonomous biathlon-inspired robot built for **UTRA Hacks 2025** - Canada's premier robotics hackathon. This robot combines precision navigation, color-based target seeking, and ball shooting to compete in a Winter Olympics-themed challenge.

![Robot Assembly](images/robot-assembly.jpeg)

## 👥 Team
[David Feldt](https://github.com/David-feldt) & Me

Mentors at UTRA Hacks 2025


## 🏆 About UTRA Hacks

**UTRA Hacks** is Canada's largest robotics hackathon, hosted annually by the [University of Toronto Robotics Association (UTRA)](https://www.utra.ca) in partnership with the U of T Robotics Institute. UTRA Hacks brings together students, mentors, and robotics enthusiasts for an intensive 2-day event focused on innovation, collaboration, and hands-on learning.

This project was developed as **mentors** at UTRA Hacks, guiding student teams through the Winter Olympics challenge while demonstrating best practices in robotics system design.


## 🎯 Challenge Overview: Winter Olympics Biathlon

Inspired by the Olympic biathlon - which combines cross-country skiing with rifle shooting - this hackathon challenged teams to build robots that demonstrate the three core pillars of robotics:

🔍 **Perception** - Color sensors, IR sensors, ultrasonic distance measurement  
🧠 **Reasoning** - Path detection, obstacle avoidance, decision making  
⚙️ **Actuation** - Motor control, servo manipulation, ball launching

### The Course

Teams designed robots to navigate a complex Winter Olympics-style course featuring:

**Section 1: Target Shooting** (25 points)
- Pick up and place a box to unlock a path
- Navigate colored ramps (straight or curved)
- Use color detection to find center of concentric target rings (blue → red → green → black)
- Shoot a ball at targets for maximum points
- Return via the ramp

**Section 2: Obstacle Course** (20 points)
- Follow a winding red path with sharp turns
- Detect and avoid black obstacles
- Complete the course as quickly as possible (bonus points for <60 seconds)

**Total possible score:** 63 points (course) + 40 points (presentation) = **103 points**

## ✨ Features

Our mentorship robot demonstrates:

- 🎨 **Color-based path navigation** - TCS3200 RGB sensor for detecting colored zones
- 🔲 **Line following** - Dual IR sensors for precise path tracking
- 🚧 **Obstacle detection** - HC-SR04 ultrasonic sensor for collision avoidance
- 🤖 **Autonomous decision making** - State machine architecture for complex behaviors
- 🦾 **Object manipulation** - Servo-controlled claw for box pickup
- 🎯 **Ball shooting mechanism** - Servo-actuated launcher for target shooting
- ⚡ **Modular calibration system** - Easy sensor tuning before competition

## 🔧 Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| Arduino Uno (USB-C) | 1 | Microcontroller |
| DC Motors | 2 | Drivetrain |
| Servo Motors | 2 | Claw & shooter |
| L298N Motor Driver | 1 | Motor control |
| HC-SR04 Ultrasonic | 1 | Distance sensing |
| TCS3200 Color Sensor | 1 | Color detection |
| IR Sensors | 2 | Line following |
| 9V Batteries | 4 | Power supply |
| Wheels | 4 | Mobility |
| Breadboard | 1 | Circuit prototyping |
| Jumper Wires | 60 | Connections |

**Kit provided by:** UTRA Hacks organizers - all components supplied to teams!

## 📁 Project Structure

```
winter-olympics-robot/
├── src/
│   ├── main.ino                    # Main control program
│   ├── config.h                    # Pin definitions and constants
│   └── sensors.h                   # Sensor reading functions
├── calibration/
│   ├── color_sensor_calibration.ino
│   ├── ir_sensor_calibration.ino
│   ├── ultrasonic_calibration.ino
│   └── motor_calibration.ino
├── tests/
│   ├── test_motors.ino
│   ├── test_color_sensor.ino
│   ├── test_ir_sensors.ino
│   ├── test_ultrasonic.ino
│   └── test_servos.ino
├── docs/
│   ├── QUICKSTART.md               # Quick start guide
│   ├── WIRING.md                   # Complete wiring guide
│   └── challenge_description.pdf   # Original hackathon package
├── images/                         # Photos and diagrams
├── README.md                       # This file
└── LICENSE                         # MIT License
```

## 🚀 Quick Start

### Prerequisites
- Arduino IDE (1.8.x or 2.x)
- USB-C cable
- All hardware components from kit

### Setup Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/winter-olympics-robot.git
   cd winter-olympics-robot
   ```

2. **Run Calibrations** (in order!)
   ```
   Open Arduino IDE → File → Open
   1. calibration/motor_calibration.ino
   2. calibration/ir_sensor_calibration.ino
   3. calibration/color_sensor_calibration.ino
   4. calibration/ultrasonic_calibration.ino
   ```

3. **Update Configuration**
   - Edit `src/config.h` with your calibrated values
   - Adjust thresholds based on calibration results

4. **Test Components**
   - Run each test sketch to verify functionality
   - Fix any issues before proceeding

5. **Upload Main Program**
   ```
   Open src/main.ino → Upload to Arduino
   ```

6. **Run on Course!**
   - Place robot at starting position
   - Power on and watch it compete!

📚 **Detailed instructions:** See [docs/QUICKSTART.md](docs/QUICKSTART.md)


## 🙏 Acknowledgments

- **UTRA** for organizing Canada's premier robotics hackathon
- **U of T Robotics Institute** for co-hosting and facility support
- **All participating student teams** for their creativity and enthusiasm
- **Fellow mentors** for sharing knowledge and supporting participants
- **Sponsors** who made UTRA Hacks 2025 possible
- **Arduino community** for excellent documentation and libraries

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🔗 Links

- 🌐 [UTRA Website](https://www.utra.ca/)
- 🎪 [UTRA Hacks](https://hackathon.utra.ca/)
- 📘 [U of T Robotics Institute](https://robotics.utoronto.ca/)
- 📚 [Arduino Documentation](https://www.arduino.cc/reference/en/)
- 💬 [UTRA Discord Community](https://discord.gg/utra)

## 🤝 Contributing

This project welcomes contributions from the robotics community! Whether you're improving documentation, optimizing code, or adding new features, your input is valuable.

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## 📬 Contact

**For questions about this project:**
- Open an issue on GitHub
- Reach out through UTRA's community channels

**Interested in joining UTRA or attending future UTRA Hacks events?**
- Visit [utra.ca](https://www.utra.ca/) to learn more
- Follow [@UofT_UTRA](https://twitter.com/UofT_UTRA) for updates
- Apply for next year's UTRA Hacks at [hackathon.utra.ca](https://hackathon.utra.ca/)

---

#UTRAHacks #Robotics #Arduino #WinterOlympics #Biathlon #AutonomousRobots