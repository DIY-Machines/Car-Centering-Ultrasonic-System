# Car Centering Ultrasonic System

## Wiring

### MAX7219 8x32 matrix to Arduino Nano

| MAX7219 | Nano |
|---|---|
| VCC | 5V |
| GND | GND |
| DIN | D11 |
| CLK | D13 |
| CS / LOAD | D10 |

### Left ultrasonic sensor to Arduino Nano

| Left sensor | Nano |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | D2 |
| ECHO | D3 |

### Right ultrasonic sensor to Arduino Nano

| Right sensor | Nano |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | D4 |
| ECHO | D5 |

## Notes

- All devices share the same **5V** supply
- All grounds must be connected together
- Sensors are mounted on opposite sides of the ramp, facing inward
