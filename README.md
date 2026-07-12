# IR Remote Controlled Smart Traffic Light System with Countdown Timer

An Arduino Uno–based traffic signal controller that combines **infrared remote control** with a **live countdown display**. An operator can manually override the signal (Red / Yellow / Green) at any time via IR remote, or let the system run a fully automatic, self-restarting countdown sequence — with the remaining seconds shown in real time on a 7-segment display.

Built and functionally verified as a Tinkercad circuit simulation, as part of a minor project during an internship at **Ediglobe**.

---

## Features

- 📡 **IR remote decoding** — NEC-style command extraction with address/inverse-command validation
- 🔴🟡🟢 **Manual override** — instantly force Red, Yellow, or Green via dedicated remote keys
- ⏱️ **Automatic countdown mode** — 60-second cycle that steps Red → Yellow → Green and restarts
- 🔢 **Live 7-segment display** — mirrors countdown value / mode status over I2C
- ⚡ **Non-blocking control loop** — uses `millis()` instead of `delay()`, so IR key presses are never missed while counting down

---

## Hardware Components

| Ref. Designator | Qty | Component                          | Role                                   |
|------------------|-----|-------------------------------------|-----------------------------------------|
| D1               | 1   | Red LED                             | Manual / "Stop" indicator               |
| D2               | 1   | Green LED                           | Manual / "Go" indicator                 |
| D3               | 1   | Yellow LED                          | Manual / "Caution" indicator            |
| R1, R2, R3       | 3   | 220 Ω Resistor                      | Current-limiting for D1–D3              |
| U5               | 1   | IR Sensor                           | Receives NEC-style remote codes         |
| U7               | 1   | Arduino Uno R3                      | Main microcontroller / logic            |
| U8               | 1   | 7-Segment Display (I2C, addr 0x70)  | Countdown / status readout              |

**Wiring:**
- IR receiver (U5) `OUT` → Arduino `D2`
- D1 (Red) via R1 → digital pin `3`
- D2 (Green) via R2 → digital pin `4`
- D3 (Yellow) via R3 → digital pin `5`
- 7-segment display U8 → `SDA` / `SCL` (I2C)

> **Note:** In firmware, the third LED channel is named `BLUE_LED` (pin 5) for readability, but it physically drives the **Yellow** LED (D3) as shown in the schematic/BOM.

---

## IR Remote Command Mapping

| Remote Key    | Constant     | Value | Action                                         |
|---------------|-------------|-------|-------------------------------------------------|
| Numeric 0     | `BTN_OFF`   | 12    | Exit auto mode, all LEDs off, display shows `0` |
| Numeric 1     | `BTN_RED`   | 16    | Force Red LED, display shows `1`                |
| Numeric 2     | `BTN_BLUE`  | 17    | Force Yellow LED, display shows `2`             |
| Numeric 3     | `BTN_GREEN` | 18    | Force Green LED, display shows `3`              |
| Play / Pause  | `BTN_PLAY`  | 5     | Toggle automatic countdown mode on/off          |

---

## How It Works

1. The Arduino continuously polls the IR receiver.
2. A decoded key either:
   - **Manual mode:** directly sets the matching LED and updates the display, or
   - **Automatic mode:** toggles `autoMode`, which starts a one-second-tick countdown loop.
3. In automatic mode, `timerCount` runs from `60 → 0`:
   - `> 30` → Red
   - `> 20` → Yellow
   - `> 0`  → Green
   - `0`    → all off, then restarts at `60`
4. The display is updated every tick, and IR key presses are still accepted at all times (non-blocking design).

---

## Libraries Used

- [`Wire.h`](https://www.arduino.cc/en/reference/wire) — I2C communication
- [`Adafruit_LEDBackpack`](https://github.com/adafruit/Adafruit-LED-Backpack-Library) — 7-segment display driver
- [`IRremote`](https://github.com/Arduino-IRremote/Arduino-IRremote) — IR signal decoding

---

## Getting Started

1. Wire the circuit as per the [circuit diagram](./docs/circuit-diagram.png) (or open the Tinkercad simulation).
2. Install the required libraries via the Arduino Library Manager.
3. Flash `IR_Traffic_Light.ino` to an Arduino Uno R3.
4. Power the circuit and control it using any standard IR remote.

---

## Circuit Diagram

![Circuit Schematic](./docs/circuit-diagram.png)

🔗 **Live simulation:** [View & simulate this circuit on Tinkercad](https://www.tinkercad.com/things/g1hDvAVVQEn-infrared-remote-control)

## Acknowledgment

This project was developed as a minor project during an internship at **Ediglobe**.

## Author

**Jostan Saldanha**
B.Tech, Electronics & Communication Engineering (Advanced Communication Technology)
NMAMIT (Nitte Deemed to be University), Karnataka
```

Want me to also add a badges row (Arduino/C/License) or a table-of-contents at the top?
