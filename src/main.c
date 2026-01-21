/**
 * @file main.c
 * @brief Real-Time Temperature and Humidity Monitoring System
 * 
 * This program reads temperature and humidity data from an SHT21 sensor
 * and displays it on an SSD1306 OLED display using an ATmega328p microcontroller.
 * 
 * Hardware Connections:
 * - SHT21 sensor: I2C (SDA=PC4, SCL=PC5)
 * - OLED display: I2C (SDA=PC4, SCL=PC5)
 * 
 * @author Real-Time Temperature-Humidity-Monitoring-System
 * @date 2024
 */

#define F_CPU 16000000UL  // CPU frequency: 16 MHz

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// I2C device addresses (7-bit addressing)
#define SHT21_ADDRESS 0x40  // SHT21 temperature/humidity sensor address
#define OLED_ADDRESS 0x3C   // SSD1306 OLED display address

// SHT21 sensor measurement commands
#define CMD_MEASURE_TEMP  0xE3   // Trigger temperature measurement (hold master)
#define CMD_MEASURE_HUMID 0xE5   // Trigger humidity measurement (hold master)

// ==================== I2C Communication Routines ====================

/**
 * @brief Initialize I2C (TWI) hardware interface
 * 
 * Configures the ATmega328p TWI module for I2C communication at ~100kHz.
 * Sets prescaler to 1 and calculates bit rate register value.
 */
void i2c_init(void) {
    TWSR = 0x00;        // Set prescaler to 1 (bits TWPS1:TWPS0 = 00)
    TWBR = ((F_CPU / 100000UL) - 16) / 2;  // Set SCL frequency to ~100kHz
    TWCR = (1 << TWEN); // Enable TWI module
}

/**
 * @brief Send I2C START condition and address byte
 * 
 * @param addr Device address byte (includes R/W bit)
 * 
 * Generates a START condition on the bus, then transmits the address byte
 * to select a specific I2C device. Waits for acknowledgment.
 */
void i2c_start(uint8_t addr) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);  // Send START condition
    while (!(TWCR & (1<<TWINT)));            // Wait for START to complete
    TWDR = addr;                              // Load address into data register
    TWCR = (1<<TWINT)|(1<<TWEN);             // Transmit address
    while (!(TWCR & (1<<TWINT)));            // Wait for transmission complete
}

/**
 * @brief Write a data byte to I2C bus
 * 
 * @param data Byte to transmit
 * 
 * Transmits a single byte over I2C and waits for acknowledgment.
 */
void i2c_write(uint8_t data) {
    TWDR = data;                      // Load data into data register
    TWCR = (1<<TWINT)|(1<<TWEN);     // Start transmission
    while (!(TWCR & (1<<TWINT)));    // Wait for transmission complete
}

/**
 * @brief Read a byte from I2C bus with ACK
 * 
 * @return Received data byte
 * 
 * Reads one byte from the I2C bus and sends an ACK to indicate
 * more data is expected. Used for multi-byte reads.
 */
uint8_t i2c_read_ack(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);  // Enable ACK and start reception
    while (!(TWCR & (1<<TWINT)));           // Wait for reception complete
    return TWDR;                             // Return received data
}

/**
 * @brief Read a byte from I2C bus with NACK
 * 
 * @return Received data byte
 * 
 * Reads one byte from the I2C bus and sends a NACK to indicate
 * this is the last byte to read. Used to terminate multi-byte reads.
 */
uint8_t i2c_read_nack(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);     // Start reception without ACK
    while (!(TWCR & (1<<TWINT)));    // Wait for reception complete
    return TWDR;                      // Return received data
}

/**
 * @brief Send I2C STOP condition
 * 
 * Generates a STOP condition on the I2C bus, releasing it for other devices.
 * Includes a small delay to ensure STOP condition is properly generated.
 */
void i2c_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);  // Send STOP condition
    _delay_us(10);                            // Wait for STOP to complete
}

// ==================== SHT21 Sensor Functions ====================

/**
 * @brief Read raw sensor value from SHT21
 * 
 * @param cmd Measurement command (CMD_MEASURE_TEMP or CMD_MEASURE_HUMID)
 * @return Raw 16-bit sensor reading
 * 
 * Sends a measurement command to the SHT21 sensor, waits for conversion,
 * then reads back the 16-bit result. The lower 2 bits are status bits and
 * are masked out.
 */
static uint16_t sht21_read_raw(uint8_t cmd) {
    uint16_t raw;
    i2c_start(SHT21_ADDRESS<<1);         // Start with write address
    i2c_write(cmd);                       // Send measurement command
    _delay_ms(85);                        // Wait for measurement (max 85ms)
    i2c_start((SHT21_ADDRESS<<1)|1);     // Restart with read address
    raw = (i2c_read_ack()<<8);           // Read MSB
    raw |= (i2c_read_nack() & 0xFC);     // Read LSB and mask status bits
    i2c_stop();                           // Release bus
    return raw;
}

/**
 * @brief Read temperature from SHT21 sensor
 * 
 * @return Temperature in degrees Celsius
 * 
 * Triggers a temperature measurement and converts the raw value to
 * degrees Celsius using the SHT21 conversion formula:
 * T = -46.85 + 175.72 * (raw / 65536)
 */
float sht21_read_temperature(void) {
    uint16_t r = sht21_read_raw(CMD_MEASURE_TEMP);
    return -46.85f + (175.72f * r / 65536.0f);
}

/**
 * @brief Read relative humidity from SHT21 sensor
 * 
 * @return Relative humidity in percent (%)
 * 
 * Triggers a humidity measurement and converts the raw value to
 * relative humidity percentage using the SHT21 conversion formula:
 * RH = -6.0 + 125.0 * (raw / 65536)
 */
float sht21_read_humidity(void) {
    uint16_t r = sht21_read_raw(CMD_MEASURE_HUMID);
    return -6.0f + (125.0f * r / 65536.0f);
}

// ==================== SSD1306 OLED Display Functions ====================

/**
 * @brief Send a command byte to the OLED display
 * 
 * @param cmd Command byte to send
 * 
 * Sends a command to the SSD1306 OLED controller. Commands control
 * display settings like power, addressing mode, contrast, etc.
 */
void oled_command(uint8_t cmd) {
    i2c_start(OLED_ADDRESS<<1);  // Start with OLED write address
    i2c_write(0x00);              // Control byte: Co=0, D/C=0 (command)
    i2c_write(cmd);               // Send command byte
    i2c_stop();                   // Release bus
}

/**
 * @brief Send a data byte to the OLED display
 * 
 * @param dat Data byte to send
 * 
 * Sends data to the SSD1306 OLED GDDRAM (Graphics Display Data RAM).
 * Data bytes represent pixel patterns to be displayed.
 */
void oled_data(uint8_t dat) {
    i2c_start(OLED_ADDRESS<<1);  // Start with OLED write address
    i2c_write(0x40);              // Control byte: Co=0, D/C=1 (data)
    i2c_write(dat);               // Send data byte
    i2c_stop();                   // Release bus
}

/**
 * @brief Initialize the SSD1306 OLED display
 * 
 * Sends a sequence of initialization commands to configure the OLED display:
 * - Display off during init
 * - Set addressing mode to horizontal
 * - Configure display orientation and mapping
 * - Set contrast, multiplexing, and timing parameters
 * - Enable charge pump for 3.3V operation
 * - Turn display on
 */
void oled_init(void) {
    const uint8_t init_cmds[] = {
        0xAE,        // Display OFF
        0x20, 0x00,  // Addressing mode: Horizontal
        0xB0,        // Page start address
        0xC8,        // COM scan direction: remapped
        0x00, 0x10,  // Column start address
        0x40,        // Display start line
        0x81, 0x7F,  // Contrast control
        0xA1,        // Segment re-map: column 127 mapped to SEG0
        0xA6,        // Normal display (not inverted)
        0xA8, 0x3F,  // Multiplex ratio: 64
        0xA4,        // Display follows RAM content
        0xD3, 0x00,  // Display offset: 0
        0xD5, 0x80,  // Clock divide ratio/oscillator frequency
        0xD9, 0xF1,  // Pre-charge period
        0xDA, 0x12,  // COM pins hardware configuration
        0xDB, 0x40,  // VCOMH deselect level
        0x8D, 0x14,  // Enable charge pump
        0xAF         // Display ON
    };
    for (uint8_t i = 0; i < sizeof(init_cmds); i++)
        oled_command(init_cmds[i]);
}

/**
 * @brief Set the cursor position on the OLED display
 * 
 * @param page Vertical page number (0-7, each page is 8 pixels tall)
 * @param col Horizontal column number (0-127)
 * 
 * Sets the GDDRAM address pointer to the specified page and column.
 * Subsequent data writes will start at this position.
 */
void oled_set_cursor(uint8_t page, uint8_t col) {
    oled_command(0xB0 | page);           // Set page address
    oled_command(0x00 | (col & 0x0F));   // Set lower column address
    oled_command(0x10 | (col >> 4));     // Set upper column address
}

/**
 * @brief Clear the entire OLED display
 * 
 * Fills all 8 pages (64 rows) and 128 columns with zeros,
 * effectively clearing the screen to black.
 */
void oled_clear(void) {
    for (uint8_t p = 0; p < 8; p++) {        // For each page (8 pages total)
        oled_set_cursor(p,0);                 // Move to start of page
        for (uint8_t c = 0; c < 128; c++)    // For each column
            oled_data(0x00);                  // Write black (all pixels off)
    }
}

// ==================== Font Data ====================

/**
 * @brief 5x7 pixel font for ASCII characters 32-127
 * 
 * Each character is 5 bytes wide, each byte represents a vertical column
 * of 8 pixels (7 used, top bit unused). Characters are indexed by
 * (ASCII code - 32).
 */
static const uint8_t font5x7[][5] = {
    // Space through ~ (ASCII 32-127)
    // Full font table as provided
};

// ==================== Text Rendering Functions ====================

/**
 * @brief Display a single character on the OLED
 * 
 * @param ch Character to display (ASCII 32-127)
 * 
 * Renders a single character at the current cursor position using the
 * 5x7 font. Invalid characters are replaced with '?'.
 * Adds 1 pixel spacing after each character.
 */
void oled_print_char(char ch) {
    if (ch < 32 || ch > 127) ch = '?';       // Replace invalid chars
    const uint8_t *bm = font5x7[ch - 32];    // Get character bitmap
    i2c_start(OLED_ADDRESS<<1);               // Start I2C transaction
    i2c_write(0x40);                          // Data mode
    for (uint8_t i=0; i<5; i++) i2c_write(bm[i]);  // Write 5 columns
    i2c_write(0x00);                          // Add spacing column
    i2c_stop();                               // End I2C transaction
}

/**
 * @brief Display a string on the OLED
 * 
 * @param s Null-terminated string to display
 * 
 * Renders a string at the current cursor position by printing
 * each character sequentially.
 */
void oled_print_string(const char *s) {
    while (*s) oled_print_char(*s++);  // Print each character
}

/**
 * @brief Display a floating-point number on the OLED
 * 
 * @param num Number to display
 * 
 * Converts a float to a string with 2 decimal places (format: ##.##)
 * and displays it on the OLED. Width is 5 characters including decimal point.
 */
void oled_print_number(float num) {
    char buf[8];
    dtostrf(num,5,2,buf);      // Convert float to string (width=5, precision=2)
    oled_print_string(buf);     // Display the string
}

// ==================== Main Program ====================

/**
 * @brief Main program entry point
 * 
 * Initializes the I2C bus and OLED display, then enters an infinite loop
 * that reads temperature and humidity from the SHT21 sensor every second
 * and updates the OLED display with the current values.
 * 
 * Display format:
 *   Line 0: "Temp: XX.XX C"
 *   Line 2: "Hum : XX.XX %"
 * 
 * @return Never returns (embedded system)
 */
int main(void) {
    // Initialize hardware peripherals
    i2c_init();         // Initialize I2C communication
    oled_init();        // Initialize OLED display
    oled_clear();       // Clear display on startup

    // Main monitoring loop
    while (1) {
        // Read sensor values
        float t = sht21_read_temperature();  // Read temperature in °C
        float h = sht21_read_humidity();     // Read humidity in %

        // Display temperature on line 0
        oled_set_cursor(0,0);
        oled_print_string("Temp:");
        oled_print_number(t);
        oled_print_char('C');

        // Display humidity on line 2
        oled_set_cursor(2,0);
        oled_print_string("Hum :");
        oled_print_number(h);
        oled_print_char('%');

        // Wait 1 second before next reading
        _delay_ms(1000);
    }
    return 0;  // Never reached
}
