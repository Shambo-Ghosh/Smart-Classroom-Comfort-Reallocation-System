# Wokwi Simulation

## Simulation Link

Replace the placeholder below with your Wokwi project URL.

```text
https://wokwi.com/projects/476307905378421761
```

---

## Purpose

The Wokwi simulation demonstrates the core logic of the Smart Classroom Comfort & Reallocation System.

The simulation includes:

- One ESP32
- Three virtual classrooms
- Three DHT22 sensors
- Three PIR sensors
- One fan indicators

---

## Simulation Goals

- Validate sensor integration
- Verify decision-making logic
- Demonstrate occupancy detection
- Demonstrate automatic ventilation control
- Provide a reproducible prototype for evaluation

---

## Note

To simplify simulation and remain compatible with the Wokwi environment, all classroom sensors are connected to a single ESP32.

The intended real-world deployment uses a distributed architecture with one ESP32 per classroom communicating through ESP-NOW.