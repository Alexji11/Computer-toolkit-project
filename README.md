# Computer Toolkit Project - StudyBuddy


## Concept
My project is a small device called StudyBuddy that helps students study better. StudyBuddy sits on the student's desk. It detects if the student is sitting at the desk and measures if the environment is good for studying (temperature, humidity and light level). An RGB LED shows the comfort level (green / yellow / red) and a buzzer gives alerts.

In addition, the Node-RED dashboards turns the numbers into useful information for the student, like how long they have studied today, when they need a break, and a comfort score. 

To sum up, StudyBuddy is a device for students who want to study more effectively and in a healthy environment. 

## Tier
Tier 2

# Tier 1 checklist
[ ] Clear creative concept
[ ] At least 2 different sensors (DHT11, LDR, ultrasonic)
[ ] At least 1 output reacting to sensor data (RGB LED, buzzer)
[ ] Arduino sends data over serial
[ ] Node-RED validates and splits messages; malformed messages are ignored
[ ] Dashboard: real-time values, at least 1 chart and 1 gauge
[ ] At least 2 live statistics (min, max, average)
[ ] At least 1 threshold alert
[ ] Raw value (0–1023) of the LDR shown next to the converted value, with explanation
[ ] Clean, commented Arduino code using functions

# Tier 2 checklist
[ ] At least 3 input components, at least 1 not used in class
[ ] At least 2 dashboard controls that change something on the Arduino
[ ] Adjustable thresholds from the dashboard
[ ] At least 2 richer statistics + reset button
[ ] Dashboard with at least 2 pages/tabs
[ ] Custom HTML/CSS styling in a template node


## Pin plan
| Component | Arduino pin |
|---|---|
| Distance sensor – Trig | D7 |
| Distance sensor – Echo | D8 |
| DHT11 – Signal | D2 |
| LDR (light sensor) | A0 |
| Push button | D4 |
| RGB LED – Red / Green / Blue | D9 / D10 / D11 |
| Buzzer | D12 |
| 5V and GND | Breadboard power rails (+ / −) |



## Wiring diagram
![Wiring diagram](docs/wiring_diagram.png)

Note: Tinkercad has no DHT11, so a TMP36 is used as a placeholder.
The real DHT11 is connected as: S → D2, VCC → 5V, GND → GND.