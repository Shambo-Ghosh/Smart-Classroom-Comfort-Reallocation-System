# Real-World Implementation

## Overview

The real-world implementation of the **Smart Classroom Comfort & Reallocation System (SCCRS)** is designed as a distributed IoT network.

Unlike the Wokwi prototype, where a single ESP32 represents all three classrooms, the real-world architecture assigns a dedicated ESP32 to each classroom.

Each classroom operates as an independent **room node** that collects local sensor data and communicates with a central **Master ESP32** using ESP-NOW.

---

## Room Node

Each classroom is equipped with:

- 1 × ESP32
- 1 × DHT22 temperature and humidity sensor
- 1 × PIR motion/occupancy sensor
- 1 × ventilation control output

The room ESP32 collects the sensor readings and handles the local classroom interface.

```text
+----------------------+
|     Room Node        |
|                      |
|      ESP32           |
|       |  |           |
|       |  +-- PIR     |
|       |              |
|       +----- DHT22   |
|                      |
|   Ventilation Output |
+----------+-----------+
           |
        ESP-NOW
           |
           V
```

---

## Master ESP32

A dedicated Master ESP32 acts as the central controller of the system.

Its responsibilities include:

- Receiving data from classroom nodes
- Maintaining room status information
- Coordinating the distributed nodes
- Processing system-level decisions
- Providing data to the monitoring/dashboard layer

The Master ESP32 communicates with individual classroom ESP32 nodes using **ESP-NOW**.

---

## Communication Architecture

The intended network follows a star-like architecture:

```text
                 +----------------+
                 |  Master ESP32  |
                 +-------+--------+
                         |
            +------------+------------+
            |            |            |
         ESP-NOW      ESP-NOW      ESP-NOW
            |            |            |
            V            V            V
      +-----------+ +-----------+ +-----------+
      | Room  1   | | Room  2   | | Room  3   |
      | ESP32     | | ESP32     | | ESP32     |
      +-----------+ +-----------+ +-----------+
```

The architecture can be expanded by adding additional classroom nodes without changing the basic concept.

---

## Sensor Data

Each room node monitors:

### DHT22

The DHT22 provides:

- Temperature
- Relative humidity

Temperature information can be used by the control logic to determine when ventilation is required.

### PIR Sensor

The PIR sensor is used to detect movement and determine the current occupancy state of the classroom.

---

## Ventilation Control

The system can combine occupancy and environmental information when determining ventilation requirements.

A simplified control concept is:

```text
             PIR Detection
                  |
                  V
          Is classroom occupied?
             /          \
           No            Yes
           |              |
           V              V
       Fan OFF      Read temperature
                          |
                          V
                  Compare threshold
                          |
                    +-----+-----+
                    |           |
               Above limit   Within limit
                    |           |
                    V           V
                 Fan ON       Fan OFF
```

The exact control thresholds and behaviour can be modified according to the requirements of the final deployment.

---

## Wokwi vs Real-World Architecture

The Wokwi simulation intentionally uses a simplified architecture.

| Aspect | Wokwi Prototype | Real-World Design |
|---|---|---|
| ESP32 | 1 | 1 per classroom + Master |
| Classrooms | 3 simulated rooms | Scalable |
| Communication | Local simulation logic | ESP-NOW |
| DHT22 | 1 per simulated room | 1 per classroom |
| PIR | 1 per simulated room | 1 per classroom |
| Ventilation | Simulated using LED | Physical control output |
| Central Controller | Same ESP32 | Dedicated Master ESP32 |

The Wokwi version is therefore a **functional proof of concept**, while the distributed ESP-NOW architecture represents the intended real-world deployment.

---

## Current Repository Artifact

The `implementation` directory currently contains the schematic representing the proposed real-world architecture:

```text
implementation/
└── real-world-schematic.png
```

The schematic documents the intended hardware connections and system structure.

---

## Future Expansion

The architecture can be extended to additional classrooms by deploying additional room nodes.

Possible future additions include:

- Additional environmental sensors
- CO₂ monitoring
- Energy consumption monitoring
- More detailed occupancy analytics
- Cloud connectivity
- Mobile monitoring
- Automated classroom allocation

---

## Implementation Status

> **Architecture:** Designed  
> **Wokwi Prototype:** Implemented  
> **Real-world schematic:** Designed  
> **Full distributed hardware deployment:** Future implementation