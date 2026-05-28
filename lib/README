# 😴 ESP32 Portable Sleep Logger (sleepController)

A lightweight, high-precision, and production-ready sleep logging system built on the **ESP32** development board using the **PlatformIO** environment. 

By utilizing two physical buttons ("Sleep" and "Wake"), the system captures sleep and wake timestamps, automatically calculates exact sleep durations, and logs well-formatted data into a Micro SD card. It is an ideal DIY device for personal health and sleep tracking.

---

## 🛠️ Hardware Requirements & Pin Mapping

* **Development Board**: ESP32 NodeMCU Development Board
* **RTC Module**: DS3231 High-Precision Real-Time Clock (with a CR2032 backup battery to keep time when powered off)
* **Storage Module**: Micro SD Card Reader Module (SPI Interface)
* **Control Elements**: 2x Physical Tactile Buttons (configured with internal pull-down resistors)

### 📌 Pin Configurations


| Component | ESP32 Pin | Description |
| --- | --- | --- |
| **SLEEP_BUTTON** | GPIO 2 | Trigger for entering sleep mode (`INPUT_PULLDOWN`) |
| **WAKE_BUTTON** | GPIO 4 | Trigger for waking up (`INPUT_PULLDOWN`) |
| **SD_CS_PIN** | GPIO 5 | Micro SD Card Chip Select (CS) |
| **SD_MOSI** | GPIO 23 | SPI Bus |
| **SD_MISO** | GPIO 19 | SPI Bus |
| **SD_CLK** | GPIO 18 | SPI Bus |
| **RTC_SDA** | GPIO 21 | I2C Data Bus |
| **RTC_SCL** | GPIO 22 | I2C Clock Bus |

---

## 🌟 Key Features & Technical Highlights

### 1. 💾 NVS Persistent Memory (Anti-Amnesia Design)
The system leverages the ESP32's built-in `Preferences` library to store state data in **Non-Volatile Storage (NVS)**. Once you press the "Sleep" button, the sleep timestamp is securely saved. **You can completely cut off the power or disconnect the power bank right after**, and the system will instantly recover your previous sleep state upon the next boot.

### 2. ⚡ Power-Bank Friendly & On-Demand Execution
Optimized for ultra-low power consumption and on-demand portability:
* **To Sleep**: Turn on the power bank ➔ Press "Sleep" ➔ **Turn off the power bank immediately**.
* **To Wake**: Turn on the power bank ➔ Press "Wake" ➔ Wait for data to save ➔ **Turn off the power bank**.
* The execution logic bypasses the "low-current auto-shutoff" safety mechanisms found in most commercial power banks during button interactions.

### 3. 📊 Automated CSV Report Generation
Upon its very first boot, the system automatically verifies and generates a file named `sleep_log.csv` in the root directory of the SD card. Every completed sleep cycle appends a new row to the log:

```csv
Sleep_Time,Wake_Time,Duration_Hours
2026/05/27 23:30,2026/05/28 07:15,7.75
```
*The final column, `Duration_Hours`, converts the sleep duration into a **decimal hour format** (e.g., 7 hours and 45 minutes becomes `7.75`), allowing you to effortlessly import the data into **Excel** or **Google Sheets** for seamless analytical calculations and visualization charting.*

---

## 💻 Software Architecture & Dependencies

This project is developed using the **PlatformIO IDE** and utilizes the following open-source libraries:
* `RTClib` (by Adafruit) - High-level wrapper for driving the DS3231 RTC module.
* `SPI`, `FS`, `SD` - Native Arduino ESP32 core libraries for robust SD card file operations.
* `Preferences` - Native ESP32 flash-memory storage library.

---

## 🚀 Getting Started

1. Clone or download this repository to your local VS Code workspace.
2. Ensure your `platformio.ini` file matches your specific ESP32 board configuration.
3. Before your first log, make sure the DS3231 RTC module is pre-calibrated to the correct local time.
4. Build and upload the firmware to your ESP32 board.
5. Open the Serial Monitor with the baud rate set to `115200`. Once you see `System Ready`, you are good to go!
