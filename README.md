🌴 WIT WIT SAWIT SPACE

Forest Monitoring Satellite System

📌 Overview

WIT WIT SAWIT SPACE is a mini satellite prototype built using ESP32 technology to monitor forest conditions and detect potential fire risks.

The system consists of:

🛰 Satellite Unit (ESP32-CAM + BMP180 + LoRa)

🌍 Ground Station Unit (ESP32 + LoRa + Alert System)

The satellite collects environmental data and sends it to Earth via LoRa communication.

🚀 Features
Satellite Unit

Temperature Monitoring (BMP180)

Atmospheric Pressure Monitoring

Risk Level Classification (LOW / MEDIUM / HIGH)

Periodic Forest Image Capture

LoRa Telemetry Transmission

Ground Station

Real-Time Data Reception

Risk Detection Alert

LED Status Indicator

Buzzer Fire Warning System

🧠 System Architecture

Satellite (ESP32-CAM)
→ Collects data
→ Captures image
→ Sends telemetry via LoRa

Ground Station (ESP32 DevKit)
→ Receives data
→ Processes risk level
→ Triggers alert if needed

🔧 Hardware Requirements
🛰 Satellite

ESP32-CAM (AI Thinker)

BMP180 Sensor

LoRa SX1278 Module (433 MHz)

Jumper Wires

3.3V Power Source

🌍 Ground Station

ESP32 DevKit

LoRa SX1278 Module (433 MHz)

LED (Red & Green)

Buzzer

Jumper Wires

🔌 Wiring Guide
LoRa → ESP32 (Both Units)
LoRa	ESP32
VCC	3.3V
GND	GND
SCK	GPIO 18
MISO	GPIO 19
MOSI	GPIO 23
NSS	GPIO 5
RST	GPIO 14
DIO0	GPIO 26
BMP180 → ESP32-CAM
BMP180	ESP32-CAM
VCC	3.3V
GND	GND
SDA	GPIO 14
SCL	GPIO 15
💻 Software Installation Guide
1️⃣ Install Arduino IDE

Download and install Arduino IDE.

2️⃣ Add ESP32 Board

Go to:

File → Preferences
Add this URL to "Additional Board Manager":

https://dl.espressif.com/dl/package_esp32_index.json

Then:

Tools → Board → Boards Manager
Search for ESP32
Install

3️⃣ Install Required Libraries

Install these libraries from Library Manager:

LoRa by Sandeep Mistry

Adafruit BMP085 / BMP180

Adafruit Unified Sensor

4️⃣ Upload Code

Open:

Satellite.ino → Upload to ESP32-CAM

GroundStation.ino → Upload to ESP32 DevKit

Make sure:

Both devices use the same LoRa frequency (433E6).

🛠 Troubleshooting
❌ BMP180 Not Detected

Check SDA and SCL pins

Ensure using 3.3V

Confirm correct wiring

❌ Camera Capture Failed (Simulation)

This may occur in simulation environments due to emulator limitations.

On real hardware, camera works normally.

❌ No LoRa Communication

Check frequency (433E6 must match)

Check antenna connection

Verify SPI wiring

Ensure both modules powered with 3.3V

📡 Transmission Format

Satellite sends data in this format:

Temp:XX,Pressure:XXXX,Risk:LEVEL

Example:

Temp:72.5,Pressure:1002,Risk:HIGH

Ground station parses the risk level and triggers alert if HIGH.

🔮 Future Development

Web Dashboard Monitoring

Cloud Data Logging

GPS Integration

AI-Based Forest Change Detection

Multi-Satellite Network

📜 License

This project is developed for educational and environmental innovation purposes.
