Smart IoT Fogponics System using Arduino, ESP8266 & Blynk


Project Overview


This project is a Smart Fogponics System designed and implemented using Arduino with ESP8266 WiFi module, capable of real-time monitoring and automated control of critical plant growth parameters.
The system continuously measures temperature, pH, and TDS (nutrient concentration) and automatically adjusts motors and foggers based on selected growth modes. All sensor data and controls are remotely accessible through the Blynk IoT mobile application.
This project demonstrates strong skills in embedded systems, IoT integration, sensor calibration, and closed-loop control, making it suitable for smart agriculture, research, and freelancing IoT solutions.



Key Features


•	Real-time monitoring of:
o	Temperature (DS18B20)
o	pH level
o	Total Dissolved Solids (TDS)
•	Cloud-connected IoT dashboard using Blynk
•	Multiple configurable growth modes
•	Automatic motor control based on pH and TDS deviation
•	Fogger ON/OFF control using RTC-based scheduling
•	Modular and scalable embedded architecture
•	Designed for low-cost smart agriculture solutions



Technologies Used


Hardware


•	Arduino Uno / Mega
•	ESP8266 WiFi Module
•	DS18B20 Temperature Sensor
•	Analog pH Sensor
•	Gravity TDS Sensor
•	RTC DS1302
•	Relay / Motor Driver
•	Fogger (Ultrasonic Mist Maker)


Software


•	Arduino IDE
•	Embedded C / Arduino Framework
•	Blynk IoT Platform
•	ESP8266 AT Commands
•	RTC and Sensor Libraries


System Architecture


Sensors (Temp / pH / TDS)
          ↓
     Arduino MCU
          ↓
 ESP8266 WiFi Module
          ↓
     Blynk Cloud
          ↓
   Mobile Dashboard

Arduino → Motor Control → Nutrient Adjustment
Arduino → Fogger Control → Root Zone Environment

Blynk Dashboard Mapping

Blynk Virtual Pin	Function

V0	Temperature Display
V1	TDS Value Display
V2	pH Value Display
V3	Mode 1 Selection
V4	Mode 2 Selection
V5	Mode 3 Selection
V6	Mode 4 Selection

Sensor Details

Temperature Sensor (DS18B20)
•	OneWire protocol
•	Accurate root-zone temperature monitoring
•	Used for fogger safety logic
TDS Sensor
•	Gravity Analog TDS Sensor
•	Temperature compensation ready
•	Nutrient concentration in ppm
pH Sensor
•	Analog pH probe
•	Linear calibration implemented in code
•	Real-time nutrient acidity control

Operating Modes
The system supports 4 predefined growth modes, each with:
•	Dedicated motor pin
•	Motor speed
•	Required pH level
•	Required TDS value
Example Mode Structure
Mode modes[4] = {
  {6, 2, 7.0, 300},
  {9, 2, 6.5, 500},
  {10,2, 7.0, 300},
  {11,2, 6.5, 500}
};
The motor automatically runs if pH or TDS deviates beyond acceptable limits and stops once conditions stabilize.

Fogger Automation Logic
•	RTC-based time tracking using DS1302
•	Fogger runs for a fixed duration
•	Cool-down period enforced
•	Temperature safety threshold implemented
•	Prevents overheating and over-fogging
This ensures optimal oxygen and nutrient delivery to plant roots.

How It Works (Flow)
1.	Sensors read temperature, pH, and TDS
2.	Values are sent to Blynk in real time
3.	User selects growth mode from mobile app
4.	System compares sensor values with target values
5.	Motors adjust nutrients automatically
6.	Foggers operate based on time and temperature
7.	Continuous feedback loop maintains stability

Setup Instructions
1.	Install Arduino IDE
2.	Install required libraries:
o	Blynk
o	ESP8266
o	DallasTemperature
o	OneWire
o	GravityTDS
o	RTC DS1302
3.	Update WiFi credentials and Blynk Auth Token
4.	Upload code to Arduino
5.	Power the system
6.	Open Blynk app and monitor live data


