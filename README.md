# NHL Stenden Intelligent Bike Light Project

## Hardware Integration: I/O Mapping and System Logic

The Intelligent Bike Light is an Arduino based safety system designed to enhance owner's visibility and comfort during daily bike rides. The device features self-adjusting front headlight and built-in turn signals, activated via handlebar buttons, to clearly communicate maneuvers to other cyclists and pedestrians. Additionaly, it utilizes an adaptive positioning mechanism that automatically tilts the beam downward upon detecting oncoming traffic, eliminating the risk of blinding them.

## Prototype Hardware Components

> **Note:** Basic assembly elements are omitted from the list below for clarity.

| Component | Qty | Description / Model |
| :--- | :---: | :--- |
| Microcontroller | 1 | Arduino Nano |
| Laser Distance Sensor | 1 | VL53L0X |
| Photoresistor | 1 | GL5528 |
| NeoPixel Ring | 1 | 12x WS2812 5050 RGBW |
| NeoPixel Stick | 2 | 4x WS2812 5050 RGB |
| Micro Servo | 1 | MG90 9g |
| Power Bank | 1 | 10000mAh |
| Push Buttons | 2 | Standard tactile switches |
| Breadboard | 1 | 400 pins |

## Pin Mapping & I/O Configuration

> **Note on Orientation:** For NeoPixel Sticks, "Left" and "Right" positions are determined by looking directly at the front of the Arduino Nano. For push buttons, "Left" and "Right" positions are determined by looking at them to form two columns. Power lines (VCC/GND) are omitted for clarity.

| Component | Arduino Pin | Pin Type | I/O Direction | Description / Connection |
| :--- | :---: | :---: | :---: | :--- |
| **NeoPixel Stick (Right)** | D2 | Digital | Output | DIN for right turn signal |
| **NeoPixel Stick (Left)** | D3 | Digital | Output | DIN for left turn signal |
| **NeoPixel Ring** | D4 | Digital | Output | DIN for main headlight assembly |
| **Push Button (Right)** | D5 | Digital | Input | Right turn signal activation switch |
| **Push Button (Left)** | D6 | Digital | Input | Left turn signal activation switch |
| **Micro Servo (MG90)** | D7 | Digital | Output | Control signal for adaptive tilting mechanism |
| **Photoresistor (GL5528)** | A3 | Analog | Input | Ambient light sensing for auto-on logic |
| **Distance Sensor (VL53L0X)** | A4 | Digital | I/O | SDA (Serial Data Line) |
| **Distance Sensor (VL53L0X)** | A5 | Digital | Input | SCL (Serial Clock Line) |

## Additional Information

* [PLACEHOLDER]

## Project Contributors

This projest was implemented by Group TD-INF-IT1C:

* Jakub Mazur (Team Leader)
* Aynur Tozluyurt
* Amin Kiani
* Jehan Master
* Joel Shoniwa
