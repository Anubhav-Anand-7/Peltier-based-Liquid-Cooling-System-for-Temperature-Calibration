# Peltier-based Liquid Cooling System for Temperature Calibration

## Overview

This project is a proof of concept for a Peltier-based liquid cooling system designed to achieve precise temperature calibration. Calibration and validation are critical aspects of control systems, ensuring accuracy, resolution, and response time. Unlike conventional systems focusing on higher temperatures, this system addresses the need for calibrating below ambient temperatures.

Using Peltier modules, which are solid-state heating/cooling devices, the system efficiently produces sub-zero cooling by extracting heat from the hot side. The design ensures accurate temperature control with a resolution of 1°C.

## Objective

The objective of this project is to:

- Develop a cost-effective and precise liquid cooling system for temperature calibration.
- Operate within a budget of Rs 4970, using specified materials and components.
- Achieve accurate temperature control with a resolution of 1°C.

## System Description

### 1. Cooling System Setup

- A copper pipe is wound tightly around a small copper tumbler for effective thermal contact.
- A 3D-printed coupler block connects:
  - **CPU cooler block** (hot side of the Peltier module).
  - **Liquid cooling block** (cold side of the Peltier module).
- Flexible silicon tubing forms a closed circuit between the copper pipe and a small circulation pump.
- Coolant fluid is introduced into the circulation line before sealing the system.

### 2. Temperature Control

- A temperature probe (**DS18B20**) measures the water temperature in the tumbler.
- A PID control system modulates:
  - **Peltier module** for cooling/heating.
  - **Coolant pump** for circulation.
- The system achieves a temperature resolution of 1°C.

### 3. User Interface

- Users set desired temperature set points using a simple interface.
- Set points are stored in local EEPROM, allowing multiple programs.
- The interface includes an **I2C OLED display** showing:
  - **Set Value (SV)**
  - **Process Value (PV)**

### 4. Working Temperature Range

- Operates within a range of **10°C to 25°C**.

## Components & Budget

| Component                                          | Qty | Unit Price (INR) | Total Cost (INR) |
| -------------------------------------------------- | --- | ---------------- | ---------------- |
| Peltier Module                                     | 1   | 250              | 250              |
| Liquid Cooling Block                               | 1   | 250              | 250              |
| Small Pump for Coolant Transfer                    | 1   | 500              | 500              |
| Coolant Fluid (100ml)                              | 1   | 250              | 250              |
| Temperature Sensor DS18B20                         | 1   | 200              | 200              |
| CPU Cooler Block (ICE-C200 V2)                     | 1   | 620              | 620              |
| 1/4” Copper Pipe                                   | 1   | 800              | 800              |
| Small Copper Tumbler                               | 1   | 300              | 300              |
| Suitable Electronics (e.g., STM32, Pi Pico, ESP32) | 1   | 1500             | 1500             |
| I2C OLED Display + Rotary Encoder                  | 1   | 300              | 300              |
| Alumina Wool Insulation                            | 1   | Custom           | 700              |
| **Total Cost**                                     |     |                  | **4970**         |

## Process Cycle

1. Wind the copper pipe tightly around the copper tumbler.
2. Connect the CPU cooler block to the hot side of the Peltier module and the liquid cooling block to the cold side.
3. Form a closed circuit by connecting the copper pipe to the circulation pump using flexible silicon tubing.
4. Introduce coolant fluid into the circulation line.
5. Insert the temperature probe into the medium (water in the tumbler).
6. Set the desired temperature set point using the user interface.
7. The PID control system modulates the Peltier module and coolant pump to achieve and maintain the set temperature.
8. The I2C OLED display continuously shows the set value (SV) and process value (PV).

## Expected Outcomes

1. **Precise Temperature Control:**

   - Accurate temperature control within a range of **10°C to 25°C**.
   - Resolution of 1°C with over/under temperature safety controls.

2. **Efficient Heat Extraction:**

   - Efficient extraction from the hot side of the Peltier module, achieving desired set points.

3. **Contamination Prevention:**

   - Closed circulation system prevents contamination and ensures coolant integrity.

4. **User-Friendly Interface:**

   - Simple interface for setting and storing multiple temperature programs.
   - Real-time display of SV and PV values.

5. **Consistent Performance:**

   - Reliable and repeatable performance over multiple cycles.

6. **Cost Efficiency:**

   - Built within a budget of Rs 4970 using the specified components.

---

This README provides a comprehensive overview of the project. For additional details or collaboration, feel free to contribute or reach out!

