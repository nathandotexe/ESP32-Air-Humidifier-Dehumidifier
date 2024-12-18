# ESP32 Autonomous Humidifier/Dehumidifier

This project showcases an **ESP32-based Autonomous Humidifier/Dehumidifier System** that dynamically monitors real-time humidity levels and adjusts the environment by controlling an atomizer module (for humidification) or a fan (for dehumidification). Ideal for smart homes, greenhouses, storage areas, or any application requiring precise humidity regulation.

---

## Features

- **Real-Time Humidity Monitoring**:
  - Continuously measures environmental humidity using a connected humidity sensor (e.g., DHT22 or similar).

- **Automatic Humidity Control**:
  - Activates the **atomizer module** when the humidity falls below the configured threshold.
  - Turns on the **fan** for dehumidification when the humidity exceeds the set limit.

- **Customizable Thresholds**:
  - Users can define minimum and maximum humidity levels directly in the code or via an external interface.

- **Energy Efficiency**:
  - Ensures efficient operation by only activating the necessary components based on the environment.

---

## Hardware Requirements

 **Microcontroller**:
  - ESP32
    
 **Sensors**:
  - Humidity Sensor (e.g., DHT22, AM2301)
  - Water Level Module

 **Actuators**:
  - Atomizer Module
  - 5v Servo

 **Additional Components**:
  - Relay module
  - L2980N Motor Driver (for advanced control on the motor speed and rotation)
  - Power supply for ESP32 and actuators
  - Breadboard and jumper wires

---

## Software Requirements

- **Arduino IDE** or **PlatformIO** (for programming the ESP32)
- **Required Libraries** (install via Arduino Library Manager or PlatformIO):
  - `DHT` (for humidity sensor communication)
  - `Adafruit_Sensor` (required by some sensors like DHT22)
  - `WiFi` (optional, for remote configuration or monitoring)
