#ifndef CONFIG_H
#define CONFIG_H

// pin and type definitions
#define DHT_PIN 4
#define DHT_TYPE DHT11
#define BTN_UP 32
#define BTN_DOWN 33
#define SDA_PIN 21
#define SCL_PIN 22

// I2C LCD
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// logging interval
#define LOG_INTERVAL 3000

#endif