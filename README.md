# esp32-espnow-telemetry

A lightweight, low-latency, transparent **MAVLink wireless telemetry bridge** between a Flight Controller (Pixhawk/ArduPilot) and Ground Control Station (Mission Planner / QGroundControl) using two ESP32 modules via the **ESP-NOW** protocol.

---

## 📌 Overview

This project replaces traditional 433MHz/915MHz telemetry modules or standard Wi-Fi setups with a fast, zero-router ESP-NOW connection. The ESP32 boards act as a transparent UART serial bridge, forwarding raw MAVLink byte streams directly between the Flight Controller and your laptop.

### Features
* **Ultra-low Latency:** Powered by Espressif's ESP-NOW protocol (peer-to-peer 2.4GHz).
* **Transparent Bridge:** No parsing or MAVLink packet manipulation overhead.
* **No Wi-Fi Router Required:** Direct point-to-point wireless communication.
* **Plug & Play:** Ground unit is detected as a standard COM/Serial port on Mission Planner or QGroundControl.

---

## 📐 System Architecture

```text
+------------------+          UART (15200)          +---------------+
|                  |  TX2 (17) ------------> RX2 (16)|               |
| Pixhawk TELEM2   |                                | ESP32S3 (Air)   |
| (ArduPilot)      |  RX2 (16) <------------ TX2 (17)|               |
+------------------+                                +-------+-------+
                                                            |
                                                   ESP-NOW (2.4 GHz)
                                                            |
+------------------+            USB / Serial        +-------+-------+
| Laptop / PC      | <----------------------------> |               |
| Mission Planner  |            (15200)             | ESP32 (Ground)|
+------------------+                                +---------------+
