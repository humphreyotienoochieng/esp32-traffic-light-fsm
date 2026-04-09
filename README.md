# esp32-climate-controller

An ESP32-based climate monitoring and control system that reads temperature and humidity from a DHT11 sensor and automatically drives a fan (cooling) and heater via PWM — with smooth ramping, OOP architecture, and CSV data logging over Serial for machine learning.

---

## Features

- **Automatic climate control** — three states: HOT, AMBIENT, COLD, with configurable thresholds
- **PWM fan and heater control** — smooth ramping between levels using a non-blocking `millis()` timer, no `delay()`
- **Object-oriented design** — fan and heater encapsulated in a reusable `PwmDevice` class (`PWMDevice.h` / `PWMDevice.cpp`)
- **CSV data logging over Serial** — outputs `timestamp_ms, temperature_C, humidity_pct, fan_pwm, heater_pwm, state` at every sensor read for capture and analysis
- **ML-ready output** — logged data intended for training scikit-learn models in Python (in progress)

---

## Hardware

| Component | Pin |
|-----------|-----|
| ESP32 DevKit V1 | — |
| DHT11 temperature/humidity sensor | GPIO 4 |
| Fan LED (blue) + 220Ω resistor | GPIO 17 |
| Heater LED (red) + 220Ω resistor | GPIO 16 |

---

## Temperature Thresholds

| Condition | Threshold | Response |
|-----------|-----------|----------|
| Extreme heat | ≥ 35°C | Fan at full speed (PWM 255) |
| High heat | ≥ 30°C | Fan at low speed (PWM 80) |
| Low cold | ≤ 5°C | Heater at low level (PWM 80) |
| Extreme cold | ≤ 0°C | Heater at full level (PWM 255) |
| Ambient | between 0°C and 30°C | All off |

---

## Project Structure

```
esp32-climate-controller/
├── src/
│   ├── main.cpp          # Main application logic
│   ├── PWMDevice.cpp     # PwmDevice class implementation
│   └── PWMDevice.h       # PwmDevice class header
├── platformio.ini
└── README.md
```

---

## Serial CSV Output

On startup the ESP32 prints a header row, then one data row every 2 seconds:

```
timestamp_ms,temperature_C,humidity_pct,fan_pwm,heater_pwm,state
2000,28.5,60.2,80,0,1
4000,36.1,58.7,255,0,0
```

State values: `0` = HOT, `1` = AMBIENT, `2` = COLD

---

## Wokwi Simulation

>Simulated using the [Wokwi VSCode extension](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode).  
Circuit diagram available in `diagram.json`.
---

## Roadmap

- [ ] Python serial capture script → `output.csv`
- [ ] scikit-learn model training on logged data
- [ ] README update with simulation link

---

## Built With

- [PlatformIO](https://platformio.org/) + Arduino framework
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- ESP32 `ledc` PWM API