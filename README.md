# 3224 LiPo Watch

This device is a small wrist watch rechargeable via USB-C that displays the time on a small 0.49" OLED display when woken by the user. The device's time can be configured by the user using two buttons (TICK and SET) and features a high-accuracy RTC IC to accurately keep track of the time. The device is run and managed by an Atmel ATtiny3224 microcontroller.

# Parts List

Here is a link to the DigiKey parts list: https://www.digikey.com/en/mylists/list/SETVN3EHG2

## General Documentation

*LED Indicators*

Red - BUILTIN
Blue - Programming Mode
Yellow - Charging (Actively, battery under full capacity)
Green - Power (USB // 3V3 {3V3 if debug switch pulled}) // Done charging (Battery at full capacity, 4.2V across terminals of LiPo expected)

Visible to user:
Yellow (for charging info)
Green (for complete charging)

*Testpoints*

Front:
EXT // 5V0 (External VCC, 5V Expected)
3V3 (Buck converted 3.3V, 3.3V Expected)
USB // 5V0 (USB-C Sourced VCC, 5V Expected)
Ground (x4, mounting holes)

Back:
BAT + (LiPo Battery Positive Terminal, 3.2V to 4.2V Expected)
BAT - (LiPo Battery Negative Terminal, Ground Expected)
Ground (x2)

*Switches // Buttons*

Front:
WAKE / TICK - Push Button on right side - wakes watch, ticks time value in set mode
SET - Push Button on left side - puts watch into set mode and sets current time digit to displayed value

Back:
RTC RST - Push Button on right middle - resets RTC IC (U4)
DEBUG INDICATORS - Slide Switch - enables debug indicator LEDs on board when turned on (Drains LiPo battery in some cases, so best to be off when packaged and not testing)

*Headers*

P1 (PROG - Programming Header)
1 - UPDI - UPDI line from programmer
2 - GND - Ground line from programmer
P2 (UART - UART Header)
1 - TXD - MCU TX
2 - RXD - MCU RX
3 - GND - Ground line from FTDI

# V1 Design

The V1 design featured a rounded PCB with a 0.49" OLED display, 64x32 pixels.

# V2 Design

Due to the 0.49" OLED failing to work properly with the 3224 MCU, the board had to be redesigned to host a larger 128x64 pixel OLED display, turning into a more square-like yet rounded PCB.