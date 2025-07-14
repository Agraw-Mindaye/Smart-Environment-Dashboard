#ifndef HELPER_H
#define HELPER_H

#include <Arduino.h>
#include <DHT.h>
#include <SD_MMC.h>
#include <LiquidCrystal_I2C.h>
#include <vector>

extern DHT dht; 
extern LiquidCrystal_I2C lcd; 
extern std::vector<String> logEntries;

extern int currIndex; 
extern int displayIndex;
extern bool autoScroll; 

// function prototypes
void logNewReading();
void loadLogEntries();
void displayCurrentLog();
void handleButtonScroll();

#endif
