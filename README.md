# IIC Temperature Monitoring System

## Overview
This project implements a low-level embedded system for real-time temperature monitoring using the **LM75B sensor over IIC (Inter-Integrated Circuit)**. The system reads raw temperature data from the sensor, processes it into human-readable form, and outputs the result through both a hardware display and a serial interface.

The focus of this project is on direct register-level programming, hardware communication protocols, and efficient data handling in embedded systems.

---

## Features
- Real-time temperature acquisition using IIC protocol  
- Direct communication with LM75B sensor registers  
- Bit-level data extraction and conversion (2’s complement handling)  
- Dual output system:  
  - 7-segment display for on-board visualization  
  - UART (Putty terminal) for serial monitoring  
- Lightweight and efficient implementation without external libraries  

---

## System Architecture
- **Sensor:** LM75B (IIC temperature sensor)  
- **Processor:** ARM-based embedded system (Zynq / Blackboard)  
- **Communication Protocols:**  
  - IIC for sensor data  
  - UART for terminal output  
- **Output Interfaces:**  
  - 7-segment display  
  - Serial terminal  

---

## Technical Details

### IIC Communication
The LM75B sensor is accessed using its internal register map:
- `0x00` → Temperature register (11-bit, 2’s complement)  
- `0x01` → Configuration register  
- `0x02` → Hysteresis threshold  
- `0x03` → Over-temperature shutdown threshold  

The system performs:
1. Pointer register write  
2. Sequential read of temperature bytes  
3. Data reconstruction from MSB and LSB  

---

### Data Processing
The raw sensor data is left-aligned and requires bit manipulation:

```c
raw = (int16_t)((hi << 8) | lo);
raw = raw >> 5;            // Extract 11-bit temperature
temperature = raw * 0.125; // Convert to Celsius

- Uses 2’s complement for signed values  
- Resolution: **0.125°C per bit**

---

## UART Output
- Configured for 115200 baud rate  
- Outputs formatted temperature readings in real time  
- Enables monitoring through tools like Putty  

---

## Display System
- Converts temperature values into BCD format  
- Drives a 7-segment display using memory-mapped I/O  
- Provides immediate visual feedback without requiring a terminal  

---

## Performance Considerations
- Minimal memory footprint  
- Efficient bitwise operations  
- No dependency on high-level libraries  
- Suitable for low-power embedded environments  

---

## Key Learnings
- Implementing IIC protocol at register level  
- Handling sensor data formats and bit alignment  
- Designing embedded systems with multiple I/O interfaces  
- Debugging hardware-software interactions  

---

## Potential Improvements
- Interrupt-driven IIC communication  
- Support for multiple sensors  
- Data logging and storage  
- Integration with higher-level monitoring systems  

---

