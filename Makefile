# Makefile for ATmega328p Temperature & Humidity Monitoring System
# 
# This Makefile builds the firmware for the ATmega328p-based temperature
# and humidity monitoring system using the SHT21 sensor and OLED display.
#
# Usage:
#   make all     - Build the project (generates .hex file)
#   make clean   - Remove all build artifacts
#   make flash   - Upload firmware to ATmega328p via Arduino bootloader
#   make fuses   - Set fuse bits for external 16MHz crystal
#
# Requirements:
#   - avr-gcc (AVR C compiler)
#   - avr-objcopy (Object file converter)
#   - avrdude (AVR programmer tool)
#   - Arduino-compatible programmer or bootloader

# ==================== Configuration ====================

# Target microcontroller
MCU = atmega328p

# CPU clock frequency (16 MHz external crystal)
F_CPU = 16000000UL

# Compiler and tools
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
AVRDUDE = avrdude

# Compiler flags
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS += -std=gnu99
CFLAGS += -funsigned-char -funsigned-bitfields
CFLAGS += -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections

# Programmer settings (adjust as needed)
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 115200

# Project name
TARGET = temp_humidity_monitor

# Source files
SRC = src/main.c

# Object files
OBJ = $(SRC:.c=.o)

# ==================== Build Targets ====================

# Default target: build everything
all: $(TARGET).hex
	@echo "Build complete!"
	@$(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

# Generate Intel HEX file from ELF
$(TARGET).hex: $(TARGET).elf
	@echo "Creating HEX file: $@"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Link object files into ELF executable
$(TARGET).elf: $(OBJ)
	@echo "Linking: $@"
	$(CC) $(CFLAGS) -o $@ $^

# Compile C source files to object files
%.o: %.c
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# ==================== Programming Targets ====================

# Upload firmware to ATmega328p
flash: $(TARGET).hex
	@echo "Flashing firmware to ATmega328p..."
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$<:i
	@echo "Flashing complete!"

# Set fuse bits for 16MHz external crystal
# Low fuse: 0xFF - Full swing crystal oscillator, 16K CK startup time
# High fuse: 0xDE - SPIEN enabled, BOOTSZ=2048 words, BOOTRST enabled
# Extended fuse: 0xFD - BOD level 2.7V
fuses:
	@echo "Setting fuse bits for 16MHz external crystal..."
	@echo "WARNING: Incorrect fuse settings can brick your device!"
	@echo "Low: 0xFF, High: 0xDE, Extended: 0xFD"
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) \
		-U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0xFD:m
	@echo "Fuses set successfully!"

# ==================== Utility Targets ====================

# Remove build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET).elf $(TARGET).hex $(OBJ)
	rm -f src/*.o
	@echo "Clean complete!"

# Display size information
size: $(TARGET).elf
	@echo "Size information:"
	$(SIZE) --mcu=$(MCU) --format=avr $<

# Help target
help:
	@echo "Available targets:"
	@echo "  make all     - Build the project"
	@echo "  make clean   - Remove build artifacts"
	@echo "  make flash   - Upload firmware to device"
	@echo "  make fuses   - Set fuse bits (use with caution!)"
	@echo "  make size    - Display memory usage"
	@echo "  make help    - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  MCU:        $(MCU)"
	@echo "  F_CPU:      $(F_CPU)"
	@echo "  Programmer: $(PROGRAMMER)"
	@echo "  Port:       $(PORT)"

# Phony targets (not actual files)
.PHONY: all clean flash fuses size help
