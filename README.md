# Smart Environment Dashboard

## Overview
The **Smart Environment Dashboard** is a real-time embedded system built around an ESP32 that monitors environmental conditions and provides local, real-time visibility while persistently logging data.

The system periodically samples temperature and humidity data, stores readings on a MicroSD card, and displays both live and historical entries on a 16×2 LCD. Users can navigate through logged entries using physical buttons with deterministic control logic.

## System Architecture

### Hardware

* ESP32 microcontroller
* Environmental sensors (temperature and humidity)
* MicroSD card module for data logging
* 16x2 LCD for real-time display
* On-board peripherals
* USB power input

### Software

* Firmware written in C++
* Real-time data acquisition loop
* Serial/WiFi-based communication to dashboard
* Data formatting and validation before transmission

## Key Features

* Periodic environmental monitoring (temperature & humidity)
* Button-controlled navigation through historical log entries
* Intelligent auto-scroll behavior with user override
* Modular firmware structure for scalability

## User Interaction & Control Logic

### Button-Based Navigation

* Users can scroll forwward and backward through stored log entries
* Entries are retrieved from memory and displayed on the LCD

### Auto-Scroll Behavior

* By default, the display automatically advances to show the latest log entry
* When the user manually scrolls:
  * Auto-scroll pauses to preserve the selected view
  * Scrolling back to the most recent entry resumes auto-scroll

# Visuals & Demo

### Circuitry
* ESP32 connected to DHT11, MicroSD module, LCD, and navigation buttons


![View Circuit](./Temp&Humidity_Circuitry.jpg)

### Live Demo

[![Watch Demo](https://img.shields.io/badge/Watch%20Demo-YouTube-red?logo=youtube&style=for-the-badge)](https://www.youtube.com/watch?v=qR6wuf3IQvo&ab_channel=Agraw)

## Future Improvements

* Introduce FreeRTOS tasks for sampling, logging, and UI
* Add SD card error detection and recovery handling
* Implement timestamping via RTC or NTP
* Optimize power consumption for long-term deployment
* Expand UI with additional menus or filtering options
