# Circuit Diagram and Hardware Setup

## Overview
This document describes the hardware connections and requirements for the Real-Time Temperature and Humidity Monitoring System using ATmega328p microcontroller.

## Component List

| Component | Part Number / Description | Quantity |
|-----------|---------------------------|----------|
| Microcontroller | ATmega328p-PU (DIP-28) | 1 |
| Temperature/Humidity Sensor | SHT21 (I2C digital sensor) | 1 |
| OLED Display | SSD1306 128x64 OLED (I2C) | 1 |
| Crystal Oscillator | 16 MHz crystal | 1 |
| Capacitors (for crystal) | 22 pF ceramic | 2 |
| Pull-up Resistors (I2C) | 4.7 kΩ | 2 |
| Decoupling Capacitor | 100 nF ceramic | 1 |
| Power Supply | 5V regulated | 1 |
| Breadboard/PCB | - | 1 |
| Jumper Wires | - | As needed |

## Pin Connections

### ATmega328p Pinout Reference
```
         ATmega328p-PU
         +---------+
   PC6  1|         |28 PC5 (SCL) [I2C Clock]
   PD0  2|         |27 PC4 (SDA) [I2C Data]
   PD1  3|         |26 PC3
   PD2  4|         |25 PC2
   PD3  5|         |24 PC1
   PD4  6|         |23 PC0
   VCC  7|         |22 GND
   GND  8|         |21 AREF
   PB6  9|         |20 AVCC
   PB7 10|         |19 PB5 (SCK)
   PD5 11|         |18 PB4 (MISO)
   PD6 12|         |17 PB3 (MOSI)
   PD7 13|         |16 PB2
   PB0 14|         |15 PB1
         +---------+
```

### SHT21 Sensor Connections

| SHT21 Pin | Function | ATmega328p Pin | Notes |
|-----------|----------|----------------|-------|
| VDD | Power | VCC (+5V) | Power supply |
| SDA | I2C Data | PC4 (Pin 27) | I2C data line with 4.7kΩ pull-up to VCC |
| SCL | I2C Clock | PC5 (Pin 28) | I2C clock line with 4.7kΩ pull-up to VCC |
| GND | Ground | GND | Ground connection |

**SHT21 I2C Address:** 0x40 (7-bit addressing)

### SSD1306 OLED Display Connections

| OLED Pin | Function | ATmega328p Pin | Notes |
|----------|----------|----------------|-------|
| VCC | Power | VCC (+5V) | Some modules require 3.3V - check your module |
| GND | Ground | GND | Ground connection |
| SDA | I2C Data | PC4 (Pin 27) | Shares I2C bus with SHT21 |
| SCL | I2C Clock | PC5 (Pin 28) | Shares I2C bus with SHT21 |

**OLED I2C Address:** 0x3C (7-bit addressing)

### Crystal Oscillator Connections

| Crystal Pin | ATmega328p Pin | Additional Components |
|-------------|----------------|----------------------|
| XTAL1 | PB6 (Pin 9) | 22 pF capacitor to GND |
| XTAL2 | PB7 (Pin 10) | 22 pF capacitor to GND |

### Power Connections

| Connection | Pin | Notes |
|------------|-----|-------|
| VCC | Pin 7 | Connect to +5V regulated supply |
| AVCC | Pin 20 | Connect to +5V (analog power) |
| GND | Pins 8, 22 | Connect to ground |
| AREF | Pin 21 | Optional: external analog reference (can leave floating) |

## Power Supply Requirements

- **Operating Voltage:** 5V DC regulated
- **Current Consumption:**
  - ATmega328p: ~15 mA (active at 16MHz)
  - SHT21 sensor: ~0.3 mA (measuring)
  - OLED display: ~20 mA (typical)
  - **Total:** ~35-40 mA typical
- **Recommended Power Source:**
  - USB power supply (5V, 500mA+)
  - Battery with 5V regulator (e.g., 7805)
  - Arduino USB connection for development

## I2C Bus Configuration

### Pull-up Resistors
The I2C bus requires pull-up resistors on both SDA and SCL lines:
- **Value:** 4.7 kΩ (typical for 5V, 100kHz I2C)
- **Connection:** One end to SDA/SCL, other end to VCC
- Some OLED modules have built-in pull-ups; check your module

### Bus Speed
- **Configured Speed:** ~100 kHz (standard mode)
- **Maximum Speed:** 400 kHz (fast mode) - adjust TWBR in code if needed

## Circuit Diagram

```
                          +5V
                           |
                           |
                    +------+------+
                    |      |      |
                  [4.7k] [4.7k] [0.1µF]
                    |      |      |
       SHT21        |      |      |
    +----------+    |      |      |
    | VDD  SDA |----+------+------|------ PC4 (SDA)
    | SCL  GND |----------+       |
    +----------+          |       +------ PC5 (SCL)
         |                |       |
      ___V___             |    ___V___
                          |            ATmega328p
       OLED               |         +-------------+
    +----------+          |    VCC  |             |  PC5 (SCL)
    | VCC  GND |----------+-------|7            28|------+
    | SDA  SCL |--------------------|27           |      |
    +----------+          |    GND  |8          22|---+  |
                          |         |             |   |  |
                         ___        |9  PB6 XTAL1 |   |  |
                                    |10 PB7 XTAL2 |   |  |
                       16MHz        +-------------+   |  |
                      Crystal            |  |         |  |
                     [22pF] [22pF]       |  |        GND |
                        |     |         ___           |  |
                       ___   ___                     ___
```

## Assembly Instructions

1. **Prepare the Breadboard/PCB**
   - Use a breadboard for prototyping or design a PCB for permanent installation

2. **Install ATmega328p**
   - Place the microcontroller on the breadboard
   - Ensure proper orientation (Pin 1 indicator)

3. **Connect Power**
   - Wire VCC (Pin 7) and AVCC (Pin 20) to +5V
   - Wire GND (Pins 8, 22) to ground
   - Add 100nF decoupling capacitor between VCC and GND close to the IC

4. **Install Crystal Oscillator**
   - Connect 16MHz crystal between PB6 and PB7
   - Add 22pF capacitor from each crystal pin to ground

5. **Connect I2C Devices**
   - Wire SHT21 SDA to PC4
   - Wire SHT21 SCL to PC5
   - Wire OLED SDA to PC4 (shared bus)
   - Wire OLED SCL to PC5 (shared bus)
   - Install 4.7kΩ pull-up resistors on SDA and SCL lines

6. **Power the I2C Devices**
   - Connect VCC pins of SHT21 and OLED to +5V
   - Connect GND pins to ground

7. **Verify Connections**
   - Double-check all connections
   - Ensure no short circuits
   - Verify power supply voltage (5V)

## Troubleshooting

### Display not working
- Check I2C pull-up resistors (4.7kΩ)
- Verify OLED address (0x3C) matches your module
- Some OLEDs require 3.3V - use level shifter if needed
- Check OLED initialization sequence

### Sensor not reading
- Verify SHT21 I2C address (0x40)
- Check sensor power supply (2.1V - 3.6V typical, 5V tolerant)
- Ensure proper I2C pull-ups
- Allow 15ms for sensor startup after power-on

### I2C Communication Errors
- Check pull-up resistor values (too high or too low can cause issues)
- Verify wire lengths (keep I2C wires short, <1 meter)
- Check for loose connections
- Ensure only one master (ATmega328p) on the bus

### System not starting
- Verify fuse bits are set correctly for external 16MHz crystal
- Check crystal oscillator connections and capacitor values
- Ensure proper power supply (stable 5V)
- Check for firmware upload issues

## Programming Interface (ISP)

For programming with ISP programmer:

| ISP Signal | ATmega328p Pin |
|------------|----------------|
| MOSI | PB3 (Pin 17) |
| MISO | PB4 (Pin 18) |
| SCK | PB5 (Pin 19) |
| RESET | PC6 (Pin 1) |
| VCC | Pin 7 |
| GND | Pin 8 |

## Safety and Best Practices

1. **Always disconnect power** before modifying connections
2. **Use proper power supply** - ensure stable 5V regulated output
3. **Check polarity** - incorrect polarity can damage components
4. **Handle ICs carefully** - avoid static discharge (use ESD protection)
5. **Start simple** - test one component at a time
6. **Measure voltages** - verify 5V supply before connecting components

## Additional Resources

- [ATmega328p Datasheet](https://www.microchip.com/wwwproducts/en/ATmega328p)
- [SHT21 Datasheet](https://www.sensirion.com/en/environmental-sensors/humidity-sensors/humidity-temperature-sensor-sht2x-digital-i2c-accurate/)
- [SSD1306 OLED Controller Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
- [I2C Bus Specification](https://www.nxp.com/docs/en/user-guide/UM10204.pdf)
