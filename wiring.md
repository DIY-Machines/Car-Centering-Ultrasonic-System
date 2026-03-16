{\rtf1\ansi\ansicpg1252\cocoartf2868
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 ## Wiring\
\
### MAX7219 8x32 matrix to Arduino Nano\
\
| MAX7219 | Nano |\
|---|---|\
| VCC | 5V |\
| GND | GND |\
| DIN | D11 |\
| CLK | D13 |\
| CS / LOAD | D10 |\
\
### Left ultrasonic sensor to Arduino Nano\
\
| Left sensor | Nano |\
|---|---|\
| VCC | 5V |\
| GND | GND |\
| TRIG | D2 |\
| ECHO | D3 |\
\
### Right ultrasonic sensor to Arduino Nano\
\
| Right sensor | Nano |\
|---|---|\
| VCC | 5V |\
| GND | GND |\
| TRIG | D4 |\
| ECHO | D5 |\
\
## Notes\
\
- All devices share the same **5V** supply\
- All grounds must be connected together\
- Sensors are mounted on opposite sides of the ramp, facing inward}