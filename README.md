# BusAutomation
A conceptual STM32 project with FreeRTOS and JSON API support.

## Purpose
- Improve the traveling experience of passengers

- Improve the driving experience of drivers

- Provide real-time data collection which can be used for later analysis

- Increases the transportation efficiency of buses

## Functions
- Detect room temperature and humidity of the bus

- Detect the light intensity of the bus

- Unlock the system using RFID

- Count the number of available seats

- Tracking bus location with GPS

- Upload the data to the server

- Display the data with LCD screen (for driver) and smartphones (for passengers)

## Getting started
To Start Server
```bash
json-server --host local_ip bus_data.json --port 3000
```
