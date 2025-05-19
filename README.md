# Temperature and Brightness Measurement System with LCD Display

This project implements a dual-sensor system that measures temperature and ambient light levels, displaying the readings on an LCD screen. The system uses an Arduino board with temperature and light sensors, providing real-time environmental monitoring with a user-friendly display.

## Features

- Real-time temperature measurement in Celsius
- Ambient light level monitoring
- Custom LCD characters for temperature (thermometer) and light (sun) symbols
- Sliding window averaging for stable readings
- 16x2 LCD display with 4-bit interface
- Updates every 2 seconds

## Hardware Requirements

- Arduino board
- 16x2 LCD display
- Temperature sensor (analog) connected to A0
- Light Dependent Resistor (LDR) connected to A1
- Supporting components:
  - Resistors as needed
  - Connecting wires
  - Breadboard

## Pin Configuration

### LCD Connections
- RS: Pin 7
- EN: Pin 6
- D4: Pin 5
- D5: Pin 4
- D6: Pin 3
- D7: Pin 2

### Sensor Connections
- Temperature Sensor: A0
- Light Sensor (LDR): A1

## Features Implementation

1. **LCD Display**
   - 4-bit interface mode
   - Custom character support for symbols
   - Clear display formatting

2. **Sensor Reading**
   - Sliding window averaging (10 samples)
   - Noise reduction
   - Smooth display updates

3. **Display Format**
   - Temperature: "🌡️: XX.X C" (First row)
   - Light Level: "☀️: XXX" (Second row)

## Usage

1. Connect the hardware according to the pin configuration
2. Upload the code to your Arduino board
3. The system will automatically start displaying:
   - Temperature readings in Celsius
   - Light level readings (raw values)
4. Display updates every 2 seconds

## Functions

- `lcdInit()`: Initializes the LCD display
- `createCustomCharacters()`: Creates custom symbols
- `readAverage()`: Implements sliding window averaging
- `lcdPrint()`: Handles string output to LCD
- Various helper functions for LCD control

## Notes

- The temperature reading is converted from raw analog value to Celsius
- Light readings are displayed as raw analog values (0-1023)
- The system uses averaging to provide stable readings
- Custom characters are stored in LCD CGRAM