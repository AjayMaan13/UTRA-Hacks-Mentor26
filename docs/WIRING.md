# Wiring Guide

Complete wiring instructions for the Winter Olympics Robot.

## Pin Connections Summary

### Arduino Uno Pins

| Pin | Component | Function |
|-----|-----------|----------|
| D2  | Motor Driver EN-B | Right motor enable (PWM) |
| D3  | Motor Driver IN4 | Right motor direction |
| D4  | Motor Driver IN3 | Right motor direction |
| D5  | Motor Driver EN-A | Left motor enable (PWM) |
| D6  | Motor Driver IN2 | Left motor direction |
| D7  | Motor Driver IN1 | Left motor direction |
| D8  | Color Sensor S0 | Frequency scaling |
| D9  | Servo (Claw) | PWM control |
| D10 | Servo (Shooter) | PWM control |
| D11 | Ultrasonic ECHO | Echo pin |
| D12 | Ultrasonic TRIG | Trigger pin |
| D13 | Color Sensor S1 | Frequency scaling |
| A0  | IR Sensor Left | Analog input |
| A1  | IR Sensor Right | Analog input |
| A2  | Color Sensor S2 | Color filter select |
| A3  | Color Sensor S3 | Color filter select |
| A4  | Color Sensor OUT | Frequency output |

## Component Wiring Details

### L298N Motor Driver

**Power Connections:**
- 12V / Battery + → 12V input
- GND → Battery -
- 5V (optional) → Arduino 5V (if using onboard regulator)

**Motor Connections:**
- OUT1, OUT2 → Left DC Motor
- OUT3, OUT4 → Right DC Motor

**Control Connections to Arduino:**
- IN1 → D7
- IN2 → D6
- EN-A → D5 (PWM)
- IN3 → D4
- IN4 → D3
- EN-B → D2 (PWM)
- GND → Arduino GND

**Important Notes:**
- Remove EN-A and EN-B jumpers for PWM speed control
- Common ground between Arduino and motor driver is essential
- Use separate power supply for motors (9V batteries)

### HC-SR04 Ultrasonic Sensor

```
HC-SR04        →    Arduino Uno
VCC            →    5V
TRIG           →    D12
ECHO           →    D11
GND            →    GND
```

**Notes:**
- Keep wires short to reduce noise
- Sensor works best perpendicular to objects
- Maximum reliable range: 200cm

### TCS3200 Color Sensor

```
TCS3200        →    Arduino Uno
VCC            →    5V
GND            →    GND
S0             →    D8
S1             →    D13
S2             →    A2 (digital mode)
S3             →    A3 (digital mode)
OUT            →    A4 (digital mode)
LED (optional) →    5V (through 220Ω resistor)
```

**Notes:**
- Optimal distance to surface: 5-10mm
- White LED improves color detection
- Avoid direct sunlight

### IR Sensors (2x)

**Left IR Sensor:**
```
IR Sensor      →    Arduino Uno
VCC            →    5V
GND            →    GND
OUT            →    A0
```

**Right IR Sensor:**
```
IR Sensor      →    Arduino Uno
VCC            →    5V
GND            →    GND
OUT            →    A1
```

**Notes:**
- Mount 5-15mm above ground
- Point LEDs perpendicular to surface
- Adjust potentiometer for optimal sensitivity

### Servo Motors (2x)

**Claw Servo:**
```
Servo          →    Arduino Uno
Brown/Black    →    GND
Red            →    5V
Orange/Yellow  →    D9
```

**Shooter Servo:**
```
Servo          →    Arduino Uno
Brown/Black    →    GND
Red            →    5V
Orange/Yellow  →    D10
```

**Notes:**
- Servos draw significant current
- Consider external 5V power supply for servos
- Add 100µF capacitor across servo power pins

### Power Distribution

**Option 1: Simple Setup**
- 2x 9V batteries → Motor driver (in parallel)
- Arduino powered via USB during development
- Arduino powered via VIN from motor driver during competition

**Option 2: Optimal Setup**
- 2x 9V batteries → Motor driver (in parallel)
- 2x 9V batteries → Separate servo power (in parallel)
- Arduino powered from motor driver 5V output
- Common ground for all components

## Complete Wiring Diagram

```
                                    ARDUINO UNO
                              ┌─────────────────────┐
   ┌──────────────────────────┤ D13            VIN  │←─── 9V (from motor driver)
   │   ┌──────────────────────┤ D12           GND  ├───→ Common GND
   │   │   ┌──────────────────┤ D11            5V  ├───→ Sensors + Servos
   │   │   │   ┌──────────────┤ D10           A0   ├───→ IR Left
   │   │   │   │   ┌──────────┤ D9            A1   ├───→ IR Right
   │   │   │   │   │   ┌──────┤ D8            A2   ├───→ Color S2
   │   │   │   │   │   │   ┌──┤ D7            A3   ├───→ Color S3
   │   │   │   │   │   │   │ ┌┤ D6            A4   ├───→ Color OUT
   │   │   │   │   │   │   │││├ D5                 │
   │   │   │   │   │   │   │││└────┐               │
   │   │   │   │   │   │   ││└─────┼────┐          │
   │   │   │   │   │   │   │└──────┼────┼──┐       │
   │   │   │   │   │   │   └───────┼────┼──┼───┐   │
   │   │   │   │   │   │           │    │  │   │   │
   │   │   │   │   │   │     ┌─────┴──┐ │  │   │   │
   │   │   │   │   │   │     │ Motor  │ │  │   │   │
   │   │   │   │   │   │     │ Driver │ │  │   │   │
   │   │   │   │   │   │     │ L298N  │ │  │   │   │
   │   │   │   │   │   │     └────────┘ │  │   │   │
   │   │   │   │   │   │                │  │   │   │
   │   │   │   │   │   └─Color S0       │  │   │   │
   │   │   │   │   └─Shooter Servo      │  │   │   │
   │   │   │   └─Claw Servo              │  │   │   │
   │   │   └─Ultrasonic ECHO             │  │   │   │
   │   └─Ultrasonic TRIG                 │  │   │   │
   └─Color S1                            │  │   │   │
                                         EN-A IN2 IN1
                                         (Left Motor Control)
```

---

*Refer to component datasheets for additional specifications.*
