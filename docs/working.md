# System Working

## Software Integration
The software is written in **Embedded C**, implementing:  
1. **I2C Communication Functions** (init, read/write, start/stop).  
2. **SHT21 Sensor Functions** (temperature & humidity reading).  
3. **OLED Display Functions** (init, clear, cursor, print).  
4. **System Logic:** Continuously read sensor data → Convert → Display.  

## Data Flow
1. Microcontroller requests data via I2C.  
2. Sensor returns raw 16-bit values.  
3. Data converted to °C and %RH.  
4. Display updated in real time on OLED.  
