# Temp_Flash(StdP)

README.md

STM32 DS18B20 Temperature Measurement & W25Q64 Power-off Data Storage Project

Project Overview

This project is developed based on STM32 Standard Peripheral Library.
It uses DS18B20 digital temperature sensor to collect real-time temperature data periodically.
Adopt W25Q64 SPI Flash to realize power-off non-volatile data storage.
The last measured temperature data is permanently saved in Flash, and will not be lost after power failure and restart.

Hardware Configuration

1. Main Control Chip: STM32F103C8T6 MCU

2. Temperature Sensor: DS18B20

3. External Flash: W25Q64 (SPI communication)

4. Function: Periodic temperature sampling + power-off data retention

Main Functions

1. Periodically read and parse temperature data from DS18B20 sensor

2. Write the latest temperature value to W25Q64 Flash memory

3. After system power-off and restart, read historical temperature data saved in Flash

4. Ensure key temperature data will not disappear after unexpected power failure

5. Realize Flash erase, write and read stable operation

Software Framework

1. STM32 Standard Library Driver

2. DS18B20 single-bus temperature driver

3. W25Q64 SPI Flash read & write management driver

4. Timing interrupt control periodic temperature measurement

5. Power-down protection data saving logic

Project Features

• Stable DS18B20 temperature acquisition accuracy

• Reliable W25Q64 non-volatile storage

• Complete data protection after sudden power failure

• Simple code structure, easy to transplant and expand

• Based on standard library, low difficulty in secondary development

Notes

1. DS18B20 bus wiring needs pull-up resistor to work normally

2. W25Q64 must erase the sector before writing new data

3. Do not frequently erase & write Flash to avoid chip damage

4. Only the latest effective temperature data is stored to save Flash space

