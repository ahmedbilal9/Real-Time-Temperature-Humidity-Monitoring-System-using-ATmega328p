# Temperature & Humidity Monitoring System (ATmega328p + Embedded C)

This repository contains the project **"AVR ATmega328p Microcontroller based Temperature and Humidity Monitoring system using Embedded C"**.

The system reads real-time **temperature and humidity** values using an **SHT21 sensor**, processes them with an **ATmega328p microcontroller**, and displays results on an **OLED display**. It also supports **IoT-based monitoring** via an ESP8266 WiFi module.

---

## 📄 Documentation
All documentation is available inside the `docs/` folder:
- description.md → Abstract & objectives  
- theory.md → Literature review, communication protocols, risk assessment, scheduling, BOM  
- hardware.md → Hardware resources  
- working.md → Software integration & system logic  
- results.md → Testing & validation  
- conclusion.md → Final summary  

---

## 💻 Code
Source code is available inside the `code/` folder:  
- `main.c` → Complete Embedded C program for SHT21 + OLED

---

## 🚀 Quick Start
1. Clone the repository:  
   ```bash
   git clone https://github.com/your-username/Temperature-Humidity-Monitoring-System.git
   cd Temperature-Humidity-Monitoring-System
   ```

2. Open `code/main.c` in your AVR development environment (AVR-GCC / Atmel Studio / CodeBlocks).  
3. Compile and upload to ATmega328p.  
4. View live readings on OLED or IoT web server (ESP8266).

---

## ✨ Features
- Low-level Embedded C implementation (no Arduino libraries).  
- I2C communication with SHT21 sensor.  
- OLED SSD1306-based real-time display.  
- Optional IoT mode via ESP8266 (web server).  
- Scalable & low-cost solution for smart monitoring.

---
