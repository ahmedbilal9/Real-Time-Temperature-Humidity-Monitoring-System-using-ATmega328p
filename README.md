# Real-Time Temperature & Humidity Monitoring System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: AVR](https://img.shields.io/badge/Platform-AVR-green.svg)](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/8-bit-mcus/avr-mcus)
[![MCU: ATmega328p](https://img.shields.io/badge/MCU-ATmega328p-red.svg)](https://www.microchip.com/wwwproducts/en/ATmega328p)

A professional embedded systems project featuring real-time temperature and humidity monitoring using an **ATmega328p microcontroller**, **SHT21 sensor**, and **SSD1306 OLED display**. This project demonstrates low-level embedded C programming without Arduino libraries, implementing direct hardware control via I2C communication.

---

## 🌟 Features

- **Pure Embedded C Implementation** - No Arduino libraries, direct register manipulation
- **I2C Communication** - Custom I2C driver implementation for ATmega328p
- **Real-Time Monitoring** - Continuous temperature and humidity readings
- **OLED Display** - Custom graphics driver for SSD1306 128x64 OLED
- **Professional Build System** - Makefile-based build automation
- **Comprehensive Documentation** - Hardware setup, circuit diagrams, and code documentation
- **Energy Efficient** - Low power consumption (~35-40mA typical)

---

## 📋 Table of Contents

- [Hardware Requirements](#-hardware-requirements)
- [Software Prerequisites](#-software-prerequisites)
- [Hardware Setup](#-hardware-setup)
- [Building the Project](#-building-the-project)
- [Flashing to Device](#-flashing-to-device)
- [Project Structure](#-project-structure)
- [Code Organization](#-code-organization)
- [Usage](#-usage)
- [Documentation](#-documentation)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🔧 Hardware Requirements

| Component | Specification | Quantity |
|-----------|--------------|----------|
| **Microcontroller** | ATmega328p-PU (DIP-28) | 1 |
| **Sensor** | SHT21 Digital Temperature/Humidity Sensor | 1 |
| **Display** | SSD1306 OLED 128x64 (I2C) | 1 |
| **Crystal** | 16 MHz Crystal Oscillator | 1 |
| **Capacitors** | 22 pF Ceramic (for crystal) | 2 |
| **Capacitors** | 100 nF Ceramic (decoupling) | 1 |
| **Resistors** | 4.7 kΩ (I2C pull-ups) | 2 |
| **Power Supply** | 5V Regulated DC | 1 |

**Estimated Total Cost:** $15-25 USD

---

## 💻 Software Prerequisites

### Required Tools

1. **AVR Toolchain**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install gcc-avr avr-libc avrdude
   
   # macOS (using Homebrew)
   brew tap osx-cross/avr
   brew install avr-gcc avrdude
   
   # Windows
   # Download and install WinAVR or AVR-GCC toolchain
   ```

2. **Make Build System**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential
   
   # macOS (included in Xcode Command Line Tools)
   xcode-select --install
   
   # Windows
   # Install MinGW or use WSL
   ```

3. **Programmer** (choose one)
   - Arduino as ISP
   - USBasp
   - AVR ISP MKII
   - Arduino bootloader (for UART programming)

### Version Requirements
- `avr-gcc` >= 5.4.0
- `avrdude` >= 6.3
- `make` >= 4.0

---

## 🔌 Hardware Setup

### Quick Start Connections

**I2C Bus (shared between SHT21 and OLED):**
- **SDA** → ATmega328p Pin 27 (PC4)
- **SCL** → ATmega328p Pin 28 (PC5)
- **Pull-up resistors**: 4.7kΩ from SDA to VCC, 4.7kΩ from SCL to VCC

**Power:**
- **VCC** → +5V (regulated)
- **GND** → Ground
- **Decoupling**: 100nF capacitor near IC

**Crystal Oscillator:**
- 16MHz crystal between PB6 (Pin 9) and PB7 (Pin 10)
- 22pF capacitor from each crystal pin to ground

📘 **For detailed circuit diagrams and pin connections, see [hardware/circuit.md](hardware/circuit.md)**

---

## 🔨 Building the Project

### 1. Clone the Repository
```bash
git clone https://github.com/ahmedbilal9/Real-Time-Temperature-Humidity-Monitoring-System-using-ATmega328p.git
cd Real-Time-Temperature-Humidity-Monitoring-System-using-ATmega328p
```

### 2. Clean Previous Builds (if any)
```bash
make clean
```

### 3. Build the Firmware
```bash
make all
```

**Expected Output:**
```
Compiling: src/main.c
Linking: temp_humidity_monitor.elf
Creating HEX file: temp_humidity_monitor.hex
Build complete!
AVR Memory Usage
----------------
Device: atmega328p

Program:    4892 bytes (15.0% Full)
(.text + .data + .bootloader)

Data:        102 bytes (5.0% Full)
(.data + .bss + .noinit)
```

### 4. Verify Build Artifacts
```bash
ls -lh *.hex
# Should show: temp_humidity_monitor.hex
```

---

## 📤 Flashing to Device

### Method 1: Using Arduino Bootloader (Recommended for Development)

1. **Configure Programmer Settings** (edit Makefile if needed)
   ```makefile
   PROGRAMMER = arduino
   PORT = /dev/ttyUSB0  # Change to your port (COM3 on Windows)
   BAUD = 115200
   ```

2. **Flash Firmware**
   ```bash
   make flash
   ```

### Method 2: Using ISP Programmer

1. **Update Makefile Settings**
   ```makefile
   PROGRAMMER = usbasp  # or avrisp2, etc.
   # PORT not needed for USBasp
   ```

2. **Flash Firmware**
   ```bash
   make flash
   ```

### Setting Fuse Bits (First Time Setup)

⚠️ **WARNING:** Incorrect fuse settings can brick your device! Only run this if you know what you're doing.

```bash
make fuses
```

This sets:
- **Low Fuse (0xFF):** External 16MHz crystal, 16K CK startup
- **High Fuse (0xDE):** SPIEN enabled, 2048-word bootloader
- **Extended Fuse (0xFD):** Brown-out detection at 2.7V

---

## 📁 Project Structure

```
Real-Time-Temperature-Humidity-Monitoring-System-using-ATmega328p/
│
├── src/                          # Source code directory
│   └── main.c                    # Main application code
│
├── include/                      # Header files (for future modularity)
│
├── docs/                         # Comprehensive documentation
│   ├── description.md            # Project abstract and objectives
│   ├── theory.md                 # Literature review and protocols
│   ├── hardware.md               # Hardware resources
│   ├── working.md                # Software integration and logic
│   ├── results.md                # Testing and validation
│   └── conclusion.md             # Final summary
│
├── hardware/                     # Hardware documentation
│   └── circuit.md                # Circuit diagrams and pin connections
│
├── Makefile                      # Build automation
├── .gitignore                    # Git ignore rules
├── LICENSE                       # MIT License
└── README.md                     # This file
```

---

## 🏗️ Code Organization

### Architecture Overview

The firmware is organized into functional modules:

1. **I2C Communication Layer** (`i2c_*` functions)
   - Low-level TWI (Two-Wire Interface) driver
   - Implements I2C start, stop, read, write operations
   - Configurable bus speed (~100kHz)

2. **SHT21 Sensor Driver** (`sht21_*` functions)
   - Temperature and humidity measurement routines
   - Automatic unit conversion (raw → °C / %)
   - Polling-based measurement with appropriate delays

3. **OLED Display Driver** (`oled_*` functions)
   - SSD1306 controller initialization
   - Cursor positioning and screen clearing
   - Custom 5x7 pixel font rendering

4. **Text Rendering Engine** (`oled_print_*` functions)
   - Character and string rendering
   - Floating-point number formatting
   - Efficient batch I2C writes

### Module Interfaces

```c
// I2C Communication
void i2c_init(void);
void i2c_start(uint8_t addr);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void i2c_stop(void);

// SHT21 Sensor
float sht21_read_temperature(void);  // Returns °C
float sht21_read_humidity(void);     // Returns %

// OLED Display
void oled_init(void);
void oled_clear(void);
void oled_set_cursor(uint8_t page, uint8_t col);
void oled_print_string(const char *s);
void oled_print_number(float num);
```

### Future Modularity

The codebase is designed for easy modularization:
- Extract I2C driver → `src/i2c.c` + `include/i2c.h`
- Extract SHT21 driver → `src/sht21.c` + `include/sht21.h`
- Extract OLED driver → `src/oled.c` + `include/oled.h`

---

## 🚀 Usage

### After Flashing

1. **Power On** - Connect 5V regulated power supply
2. **Wait for Initialization** - Display will clear and show initial readings
3. **Monitor Readings** - Display updates every second

### Display Format

```
Temp: 23.45C
Hum : 56.78%
```

- **Line 0:** Temperature in degrees Celsius
- **Line 2:** Relative humidity in percent
- **Update Rate:** 1 second

### Expected Ranges

- **Temperature:** -40°C to +125°C (sensor range)
- **Humidity:** 0% to 100% RH
- **Accuracy:** ±0.3°C (temp), ±2% RH (humidity)

---

## 📚 Documentation

Comprehensive documentation is available in the `docs/` folder:

- **[description.md](docs/description.md)** - Project abstract, objectives, and problem statement
- **[theory.md](docs/theory.md)** - Literature review, I2C protocol, risk assessment, BOM
- **[hardware.md](docs/hardware.md)** - Hardware components and specifications
- **[working.md](docs/working.md)** - Software integration, flowcharts, system logic
- **[results.md](docs/results.md)** - Testing procedures, validation results
- **[conclusion.md](docs/conclusion.md)** - Project summary, outcomes, future work

Hardware-specific documentation:

- **[hardware/circuit.md](hardware/circuit.md)** - Complete circuit diagrams, pin connections, troubleshooting

---

## 🐛 Troubleshooting

### Build Issues

**Problem:** `avr-gcc: command not found`
- **Solution:** Install AVR toolchain (see [Software Prerequisites](#-software-prerequisites))

**Problem:** `make: *** No rule to make target`
- **Solution:** Ensure you're in the project root directory

### Flashing Issues

**Problem:** `avrdude: ser_open(): can't open device`
- **Solution:** Check PORT setting in Makefile, verify programmer connection

**Problem:** `Device signature doesn't match`
- **Solution:** Verify MCU is ATmega328p, check power and connections

### Hardware Issues

**Problem:** Display not working
- **Solution:** 
  - Verify I2C pull-up resistors (4.7kΩ)
  - Check OLED address (0x3C)
  - Ensure proper power supply (5V)

**Problem:** Incorrect temperature/humidity readings
- **Solution:**
  - Verify SHT21 connections
  - Check sensor I2C address (0x40)
  - Allow sensor to stabilize (15ms after power-on)

**Problem:** System not starting
- **Solution:**
  - Check fuse bits (external crystal setting)
  - Verify 16MHz crystal connections
  - Check power supply stability

📘 **For more troubleshooting tips, see [hardware/circuit.md](hardware/circuit.md)**

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Fork the repository**
2. **Create a feature branch** (`git checkout -b feature/amazing-feature`)
3. **Commit your changes** (`git commit -m 'Add amazing feature'`)
4. **Push to the branch** (`git push origin feature/amazing-feature`)
5. **Open a Pull Request**

### Development Guidelines

- Follow existing code style (K&R style, 4-space indentation)
- Add comments for complex logic
- Update documentation for new features
- Test on hardware before submitting

---

## 📝 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

```
Copyright (c) 2024 Real-Time Temperature-Humidity-Monitoring-System
```

---

## 👥 Authors

- **Ahmed Bilal** - [ahmedbilal9](https://github.com/ahmedbilal9)

---

## 🙏 Acknowledgments

- AVR-GCC toolchain developers
- AVR-libc contributors
- Sensirion for SHT21 sensor documentation
- Solomon Systech for SSD1306 datasheet
- Open-source embedded systems community

---

## 📧 Contact

For questions, issues, or suggestions:
- **GitHub Issues:** [Create an issue](https://github.com/ahmedbilal9/Real-Time-Temperature-Humidity-Monitoring-System-using-ATmega328p/issues)
- **GitHub Discussions:** [Join discussions](https://github.com/ahmedbilal9/Real-Time-Temperature-Humidity-Monitoring-System-using-ATmega328p/discussions)

---

## 🔗 Related Projects

- [AVR I2C Library](https://github.com/g4lvanix/I2C-master-lib)
- [ATmega328p Projects](https://github.com/topics/atmega328p)
- [Embedded C Examples](https://github.com/topics/embedded-c)

---

**Made with ❤️ for the embedded systems community**
