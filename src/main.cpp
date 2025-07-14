#include "config.h"
#include "helper.h"

DHT dht(DHT_PIN, DHT_TYPE); // DHT11
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS); // 16x2 LCD display
std::vector<String> logEntries; // vector to store log entries

int currIndex = 0; // index for logging new entries
int displayIndex = 0; // index for displaying entries to LCD
bool autoScroll = true; // default scroll feature
unsigned long lastLog = 0;

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
    delay(1000);

    // init SD card
    if (!SD_MMC.begin("/sdcard", true)) {  // 1-bit mode for SD card
        lcd.clear();
        lcd.print("SD card init failed!");
        while (true);
    }

    // SD_MMC.remove("/log.csv");

    if (logEntries.empty()) {
        lcd.clear();
        lcd.print("Waiting for logs...");
        delay(1000);
    } else {
        currIndex = logEntries.size() - 1;
        displayCurrentLog();
    }
}

void loop() {
    handleButtonScroll();

    if (millis() - lastLog >= LOG_INTERVAL) {
        lastLog = millis();
        logNewReading();
    }
}
