#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

// I2C and device addresses
#define SHT21_ADDRESS 0x40
#define OLED_ADDRESS 0x3C

// SHT21 commands
#define CMD_MEASURE_TEMP  0xE3
#define CMD_MEASURE_HUMID 0xE5

// ------------------ I2C Routines ------------------
void i2c_init(void) {
    TWSR = 0x00;        // Prescaler = 1
    TWBR = ((F_CPU / 100000UL) - 16) / 2;  // SCL ≈100kHz
    TWCR = (1 << TWEN);
}

void i2c_start(uint8_t addr) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    TWDR = addr;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

void i2c_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    _delay_us(10);
}

// --------------- SHT21 Sensor -------------------
static uint16_t sht21_read_raw(uint8_t cmd) {
    uint16_t raw;
    i2c_start(SHT21_ADDRESS<<1);
    i2c_write(cmd);
    _delay_ms(85);
    i2c_start((SHT21_ADDRESS<<1)|1);
    raw = (i2c_read_ack()<<8);
    raw |= (i2c_read_nack() & 0xFC);
    i2c_stop();
    return raw;
}

float sht21_read_temperature(void) {
    uint16_t r = sht21_read_raw(CMD_MEASURE_TEMP);
    return -46.85f + (175.72f * r / 65536.0f);
}

float sht21_read_humidity(void) {
    uint16_t r = sht21_read_raw(CMD_MEASURE_HUMID);
    return -6.0f + (125.0f * r / 65536.0f);
}

// ------------- SSD1306 OLED -------------------
void oled_command(uint8_t cmd) {
    i2c_start(OLED_ADDRESS<<1);
    i2c_write(0x00);
    i2c_write(cmd);
    i2c_stop();
}

void oled_data(uint8_t dat) {
    i2c_start(OLED_ADDRESS<<1);
    i2c_write(0x40);
    i2c_write(dat);
    i2c_stop();
}

void oled_init(void) {
    const uint8_t init_cmds[] = {
        0xAE,0x20,0x00,0xB0,0xC8,0x00,0x10,0x40,
        0x81,0x7F,0xA1,0xA6,0xA8,0x3F,0xA4,0xD3,
        0x00,0xD5,0x80,0xD9,0xF1,0xDA,0x12,0xDB,
        0x40,0x8D,0x14,0xAF
    };
    for (uint8_t i = 0; i < sizeof(init_cmds); i++)
        oled_command(init_cmds[i]);
}

void oled_set_cursor(uint8_t page, uint8_t col) {
    oled_command(0xB0 | page);
    oled_command(0x00 | (col & 0x0F));
    oled_command(0x10 | (col >> 4));
}

void oled_clear(void) {
    for (uint8_t p = 0; p < 8; p++) {
        oled_set_cursor(p,0);
        for (uint8_t c = 0; c < 128; c++)
            oled_data(0x00);
    }
}

// --------- Custom 5×7 Font (ASCII 32–127) ----------
static const uint8_t font5x7[][5] = {
    // Full font table as provided
};

// ------------- Text Rendering ----------------
void oled_print_char(char ch) {
    if (ch < 32 || ch > 127) ch = '?';
    const uint8_t *bm = font5x7[ch - 32];
    i2c_start(OLED_ADDRESS<<1);
    i2c_write(0x40);
    for (uint8_t i=0; i<5; i++) i2c_write(bm[i]);
    i2c_write(0x00);
    i2c_stop();
}

void oled_print_string(const char *s) {
    while (*s) oled_print_char(*s++);
}

void oled_print_number(float num) {
    char buf[8];
    dtostrf(num,5,2,buf);
    oled_print_string(buf);
}

// -------------------- Main --------------------
int main(void) {
    i2c_init();
    oled_init();
    oled_clear();

    while (1) {
        float t = sht21_read_temperature();
        float h = sht21_read_humidity();

        oled_set_cursor(0,0);
        oled_print_string("Temp:");
        oled_print_number(t);
        oled_print_char('C');

        oled_set_cursor(2,0);
        oled_print_string("Hum :");
        oled_print_number(h);
        oled_print_char('%');

        _delay_ms(1000);
    }
    return 0;
}
