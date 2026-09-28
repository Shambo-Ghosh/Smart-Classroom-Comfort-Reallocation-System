# System Workflow

## Operational Flow

```text
PIR Sensor Detects Motion
            |
            V
Determine Occupancy Status
            |
            V
Read Temperature from DHT22
            |
            V
Evaluate Conditions
            |
            V
Control Ventilation Device
            |
            V
Update Dashboard
            |
            V
Store Current Room Status
```

---

## Detailed Workflow

### Step 1: Occupancy Detection

The PIR sensor continuously monitors movement inside the classroom.

Possible states:

- Occupied
- Unoccupied

---

### Step 2: Temperature Monitoring

The DHT22 sensor periodically measures classroom temperature.

The temperature value is sent to the controller for processing.

---

### Step 3: Decision Making

The ESP32 evaluates:

- Occupancy status
- Temperature reading

If the classroom is occupied and the temperature exceeds the predefined threshold, ventilation is activated.

---

### Step 4: Ventilation Control

The controller updates the fan status.

Possible states:

- Fan ON
- Fan OFF

---

### Step 5: Dashboard Update

The dashboard receives updated information including:

- Room occupancy
- Temperature
- Fan status

---

## Real-World Deployment Workflow

```text
Room Node
   |
   V
Collect Sensor Data
   |
   V
ESP-NOW Transmission
   |
   V
Master ESP32
   |
   V
Decision Processing
   |
   V
Dashboard Update
```

---

## Benefits

- Automated classroom monitoring
- Reduced manual intervention
- Improved comfort
- Lower energy consumption
- Real-time visibility of room status