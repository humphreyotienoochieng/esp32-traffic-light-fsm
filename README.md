# ESP32 Traffic Light FSM with Emergency Override and PWM Dimming

A dual-channel traffic light controller built on the ESP32, implementing a Finite State Machine (FSM) with emergency override, LDR-based automatic day/night brightness dimming via PWM, and non-blocking timing using `millis()`.

---

## Features

- Dual traffic light control (Channel A and Channel B)
- Finite State Machine (FSM) with 7 states
- Emergency override via push button (all-red immediately)
- Automatic day/night mode switching using an LDR sensor
- Smooth PWM brightness dimming (255 for day, 80 for night)
- Non-blocking timing using `millis()` instead of `delay()`

---

## FSM State Diagram
```
A_GREEN → A_YELLOW → ALLRED1 → B_GREEN → B_YELLOW → ALLRED2 → A_GREEN
                                    ↑
                            EMERGENCY_RED (button press, any state)
```

---

## Hardware

| Component              | Quantity |
|------------------------|----------|
| ESP32 DevKit V1        | 1        |
| Red LED                | 2        |
| Green LED              | 2        |
| Yellow LED             | 2        |
| 220Ω Resistor          | 6        |
| LDR Photoresistor      | 1        |
| Push Button            | 1        |

---

## Pin Mapping

| Pin | Function          |
|-----|-------------------|
| 16  | Red LED A         |
| 17  | Green LED A       |
| 18  | Yellow LED A      |
| 19  | Red LED B         |
| 21  | Green LED B       |
| 22  | Yellow LED B      |
| 34  | LDR (Analog In)   |
| 23  | Push Button       |

---

## Wokwi Simulation

A `diagram.json` is included for simulation in [Wokwi](https://wokwi.com). Open the project in Wokwi and load the diagram to simulate the full circuit.

---

## Getting Started

### Requirements
- [PlatformIO](https://platformio.org/) with VS Code
- ESP32 DevKit V1 board

### Build and Upload
```bash
pio run --target upload
```

### Monitor Serial Output
```bash
pio device monitor
```

---

## Timing Configuration

| State         | Duration |
|---------------|----------|
| Green         | 5000 ms  |
| Yellow        | 2000 ms  |
| All Red       | 5000 ms  |
| Emergency Red | 5000 ms  |

---

## Brightness Levels

| Mode  | PWM Value |
|-------|-----------|
| Day   | 255       |
| Night | 80        |

LDR thresholds: Night trigger `< 1500`, Day trigger `> 1800`

---

## Project Structure
```
├── src/
│   └── main.cpp
├── diagram.json
├── platformio.ini
└── README.md
```

---

## Built With

- ESP32 Arduino framework
- PlatformIO
- Wokwi (simulation)