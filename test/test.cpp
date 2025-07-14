#include <Arduino.h>
#include <DHT.h>
#include <SD_MMC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <vector>
#include "config.h"

DHT dht(DHT_PIN, DHT_TYPE); // temp and humidity sensor
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS); // lcd 16x2 display

std::vector<String> logEntries; // vector to stroe log entries
int currIndex = 0;
unsigned long lastLog = 0;

void logNewReading();
void loadLogEntries();
void displayCurrentLog();
void handleButtonScroll();

void setup() {
    Serial.begin(115200); // serial monitoring

    // init DHT11
    dht.begin(); 

    // init buttons
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);

    // init LCD
    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Initializing...");

    // init SD card
    if (!SD_MMC.begin("/sdcard", true)) {  // 1-bit mode for SD card
        lcd.clear();
        lcd.print("SD card init failed!");
        while (true);  // stop if SD fails
    }

    lcd.clear();
    lcd.print("SD Ready!");
    delay(1000);

    loadLogEntries();
    displayCurrentLog();
}

// function to log new reading to SD card
void logNewReading() {

    // read temp and humidity
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT11");
        return;
    }

    // string fromatting for ouput 
    String logEntry = String(millis() / 1000) + "," + String(temp) + "," + String(hum);

    // open 'log.csv' file for writing
    File file = SD_MMC.open("/log.csv", FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file");
        return;
    }

    // write entry log to SD card
    file.println(logEntry);
    file.close();

    // print entry for serial debugging
    Serial.print("Logged: ");
    Serial.println(logEntry);

    logEntries.push_back(logEntry); // add new entry to vector
}

// function to load log entries from SD card
void loadLogEntries() {
    logEntries.clear();

    File file = SD_MMC.open("/log.csv");
    if (!file) {
        Serial.println("Failed to open log file");
        lcd.clear();
        lcd.print("No log file");
        return;
    }

    while (file.available()) {
        logEntries.push_back(file.readStringUntil('\n'));
    }

    file.close();
    currIndex = logEntries.size() > 0 ? logEntries.size() - 1 : 0;
}

// function to display current log to display
void displayCurrentLog() {
    lcd.clear();
    if (logEntries.empty()) {
        lcd.setCursor(0,0);
        lcd.print("No entries fond");
        return;
    }

    lcd.setCursor(0,0);
    lcd.print("Log entry: " + String(currIndex + 1));
    lcd.setCursor(0,1);
    lcd.print(logEntries[currIndex].substring(0,16)); // trim to include first 16 chars
}

// function to handle button scroll on display
void handleButtonScroll() {
    static unsigned long lastPress = 0;
    if (millis() - lastPress < 200) return;

    if (digitalRead(BTN_UP) == LOW && currIndex > 0) {
        currIndex--;
        displayCurrentLog();
        lastPress = millis();
    }

    if (digitalRead(BTN_DOWN) == LOW && currIndex < (int)logEntries.size() - 1) {
        currIndex++;
        displayCurrentLog();
        lastPress = millis();
    }
}

void loop() {

    handleButtonScroll();

    // if enough time has passed (5 secs), log new reading
    if (millis() - lastLog >= LOG_INTERVAL) {
        lastLog = millis();
        logNewReading();
    }
}
