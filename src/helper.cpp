#include "config.h"
#include "helper.h"

// function to log new reading to SD card
void logNewReading() {

    // read temp and humidity
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT11");
        return;
    }

    // string fromatting for output 
    String logEntry = "T:" + String(temp) + "," + "H:" + String(hum);

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

    currIndex = logEntries.size() - 1;

    if (autoScroll) {  
        displayIndex = currIndex; // automatically scroll to latest log if autoscroll is enabled
        displayCurrentLog();
    }
}

// function to load log entries from SD card
void loadLogEntries() {
    logEntries.clear();

    File file = SD_MMC.open("/log.csv");
    if (!file) {
        Serial.println("Failed to open log file");
        //lcd.clear();
        //lcd.print("Waiting for logs...");
        return;
    }

    while (file.available()) {
        logEntries.push_back(file.readStringUntil('\n')); // append entry to log file
    }

    file.close();
}

// function to display current log to display
void displayCurrentLog() {
    lcd.clear();
    if (logEntries.empty()) {
        lcd.setCursor(0,0);
        lcd.print("No entries found");
        return;
    }

    lcd.setCursor(0,0);
    lcd.print("Log entry: " + String(displayIndex + 1));
    lcd.setCursor(0,1);
    lcd.print(logEntries[displayIndex].substring(0,16)); // trim to include first 16 chars
}

// function to handle button scroll on display
void handleButtonScroll() {
    static unsigned long lastPress = 0;
    static bool btnUpPrev = HIGH;
    static bool btnDownPrev = HIGH;

    bool btnUpNow = digitalRead(BTN_UP);
    bool btnDownNow = digitalRead(BTN_DOWN);

    if (millis() - lastPress >= 200) {
        // UP button pressed
        if (btnUpPrev == HIGH && btnUpNow == LOW && displayIndex > 0) {
            displayIndex--;
            autoScroll = false; // turn off autoscroll
            lastPress = millis();
            displayCurrentLog();
        }

        // DOWN button pressed
        else if (btnDownPrev == HIGH && btnDownNow == LOW && displayIndex < (int)logEntries.size() - 1) {
            displayIndex++;
            lastPress = millis();
            displayCurrentLog();
            
            // re-enable autoscroll if user reaches end of log entries
            if (displayIndex == (int)logEntries.size() - 1) autoScroll = true;
        }
    }

    btnUpPrev = btnUpNow;
    btnDownPrev = btnDownNow;
}