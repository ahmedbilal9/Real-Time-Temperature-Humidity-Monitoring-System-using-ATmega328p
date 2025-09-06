# Theory & Background

## Literature Review
- **Temperature & Humidity Measurement Methods:** Capacitive, resistive, thermal conductivity.  
- **Sensors:** DHT11, DHT22, SHT3x series, LM35.  
- **Calibration & Accuracy:** Software correction, averaging, error compensation.  
- **Display Methods:** LCD, OLED, LED.  
- **Data Logging:** EEPROM, SD cards, cloud.  
- **Protocols:** I2C (multi-device, slower), SPI (fast, more pins), One-Wire (simple, slow), Analog (easy, noisy).  

---

## Communication Protocols – Pros & Cons
| Protocol | Advantages | Disadvantages |
|----------|-------------|---------------|
| I2C | Fewer pins, multi-device | Slower than SPI |
| SPI | High speed | More pins needed |
| One-Wire | Simple | Slower |
| Analog | Easy | Noise prone |

---

## Resource Allocation & Scheduling
- **Sensor Selection & Circuit Design:** ATmega328p, sensors, simulated in Proteus.  
- **Firmware Development:** AVR-GCC, Embedded C.  
- **Display Implementation:** OLED drivers.  
- **Power Management:** Battery & regulators.  

Time scheduling was tracked using a Gantt chart (MS Project / Instagantt).  

---

## Bill of Materials (BOM)
| Component | Cost (PKR) |
|-----------|------------|
| LEDs | 30 |
| Pushbuttons | 70 |
| OLED Display | 380 |
| I2C PCF8574 | 130 |
| Jumper Wires | 200 |
| ATmega328p Arduino | 1200 |
| Breadboard | 170 |
**Total Cost = 2180 PKR** (self-funded).  

---

## Risk Assessment
- **Technical Risks:** Sensor malfunction, communication failure, code bugs.  
- **Operational Risks:** WiFi connectivity issues, power fluctuations, component failures.  
- **Schedule Risks:** Component delivery delays, integration delays.  
- **External Risks:** Environment variations, team availability issues.  

Mitigation strategies included calibration, backup sensors, debugging, regulated supply, ordering early, and documentation.  
