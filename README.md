# ADC Demo projects for dsPIC33CH and Curiosity board

Hardware Requirements:
- [dsPIC33CH Curiosity Development Board](https://www.microchip.com/en-us/development-tool/DM330028-2)
- Micro USB cable (there is no one included with board!)
- ensure that you downloaded data-sheet from:
  - https://ww1.microchip.com/downloads/en/DeviceDoc/DS50002801%20-%20dsPIC33CH%20Curiosity%20Development%20Board%20Users%20Guide.pdf
- Notice dsPIC type from data-sheet:
  - [dsPIC33CH512MP508](https://www.microchip.com/en-us/product/dsPIC33CH512MP508)

Here is brief overview of I/O peripherals - excluding DC/DC converter parts:

| Peripheral | dsPIC33CH pin and/or port |
| --- | --- |
| S1 push-button | RE7 |
| S2 push-button | RE8 |
| S3 push-button | RE9 |
| S4 push-button | /MCLR (reset) |
| R/G/B LED | RB14/RD7/RD5 |
| LED1 red | RE0 |
| LED2 red | RE1 |
| 10kOhm pot | RA0 |


Software Requirements:

- MPLAB X IDE `v6.00`

  - Do NOT use MPLAB X IDE v6.05 - it will freeze forever when Master/Slave projects are associated!
  - Do NOT use MPLAB X IDE v6.10 - it wil fail to use PKOB (PicKit on-board programmer) on Curiosity
    board!

- MCC Plugin 5.1.17
- DFP: `dsPIC33CH-MP_DFP `v1.12.352` (upgraded from 1.10.223 that is included with MPLAB X IDE v6.00)
- XC16 `v2.10`

# Current projects

* [ADC1.x](ADC1.x) - testing ADC ... TODO

  - now uses only master core at 180 Mhz / 90 MIPS
  - toggle LED1 with rate 100 ms from TMR1 interrupt
  - toggle LED2 from main loop with 1s rate.
  - ADC input uses AN14/RC2 - put some signal there, limits are:
    - minimum Voltage: 0V
    - maximum Voltage: 3.3V (same as Vdd)
    - ADC is 12-bit, range 0 to 4095 (0xfff)
  - DAC Output, unfortunately it is allowed to use only range 205 to 3890 instead of 0 to 4095 - so 
    amplitude is a bit smaller.
  - GPIO debug output (toggles on each finish of ADC conversion) uses RB11
  - currently 1 ADC conversion takes 1.69 µs
  - UART, output ADC stats every second, set: 115200 bps, 8 bit data, 1 stop, no parity, no flow


