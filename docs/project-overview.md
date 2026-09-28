# Smart Classroom Comfort & Reallocation System (SCCRS)

## Overview

The Smart Classroom Comfort & Reallocation System (SCCRS) is an IoT-based solution designed to improve classroom comfort, reduce energy wastage, and provide classroom occupancy visibility.

The system monitors:

- Classroom occupancy using PIR sensors
- Classroom temperature using DHT22 sensors

Based on these inputs, the system automatically controls ventilation devices and updates a dashboard with room status information.

---

## Problem Statement

Educational institutions often face challenges such as:

- Poor classroom ventilation
- Unnecessary energy consumption
- Lack of real-time room occupancy information
- Difficulty identifying available classrooms

These issues can negatively affect student comfort and operational efficiency.

---

## Proposed Solution

SCCRS continuously monitors classroom conditions and performs automated decision-making.

The system:

1. Detects whether a classroom is occupied.
2. Measures room temperature.
3. Controls ventilation when required.
4. Updates a monitoring dashboard.
5. Provides room availability information.

---

## Prototype Architecture (Wokwi Simulation)

The simulation uses:

- 1 ESP32
- 3 Virtual Classrooms
- 3 DHT22 Sensors
- 3 PIR Sensors
- 1 Fan Indicators (LED)

The simulation demonstrates the complete control logic while keeping hardware requirements minimal.

---

## Real-World Deployment Architecture

The intended deployment architecture uses:

### Per Classroom

- 1 ESP32
- 1 DHT22 Sensor
- 1 PIR Sensor
- 1 Ventilation/Fan Control Unit

### Central Controller

- 1 Master ESP32

### Communication

- ESP-NOW Protocol

The Master ESP32 receives classroom data, manages room status information, and interfaces with the dashboard.

---

## Technologies Used

### Hardware

- ESP32
- DHT22
- PIR Sensor

### Software

- Arduino IDE
- Wokwi Simulator
- HTML Dashboard

### Communication

- ESP-NOW

---

## Objectives

- Improve classroom comfort
- Reduce unnecessary energy usage
- Monitor occupancy in real time
- Demonstrate scalable IoT architecture
- Enable future smart campus integration

---

## Expected Impact

The project demonstrates how low-cost IoT devices can be used to improve infrastructure management in educational institutions while maintaining scalability for future expansion.