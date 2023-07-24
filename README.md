# ADC Demo projects for dsPIC33CH and Curiosity board

Hardware Requirements:

- [dsPIC33CH Curiosity Development Board](https://www.microchip.com/en-us/development-tool/DM330028-2)
- [Digilent Analog Discovery 2](https://digilent.com/shop/analog-discovery-2-100ms-s-usb-oscilloscope-logic-analyzer-and-variable-power-supply/)
- [BNC Adapter for AD2](https://digilent.com/shop/bnc-adapter-for-analog-discovery/)
- [Pair of probes](https://digilent.com/shop/bnc-oscilloscope-x1-x10-probes-pair/)
- 2x Micro USB cable (there is no one included with board!)

  - 1st to connect PKOB (PICkit on Board) Programmer/Debugger
  - 2nd to see output from UART

- ensure that you downloaded data-sheet from:

  - https://ww1.microchip.com/downloads/en/DeviceDoc/DS50002801%20-%20dsPIC33CH%20Curiosity%20Development%20Board%20Users%20Guide.pdf
- Notice dsPIC type from data-sheet:
  - [dsPIC33CH512MP508](https://www.microchip.com/en-us/product/dsPIC33CH512MP508)

Here is brief overview of used onboard I/O peripherals:

| Peripheral | dsPIC33CH pin and/or port |
| --- | --- |
| LED1 red | RE0 |
| LED2 red | RE1 |
| UART | `RC10_RXB` |

Software Requirements:

- MPLAB X IDE `v6.10`

  - Do NOT use MPLAB X IDE v6.05 - it will freeze forever when Master/Slave projects are associated!
  - Plugin Updates REQUIRED for MPLAB X IDE v6.10, otherwise it will not find Curiosity PKOB (PICkit on Board)
    programmer/debugger

- MCC Plugin 5.3.7 with PLIB 5.5.7
- DFP: `dsPIC33CH-MP_DFP `v1.12.352` (upgraded from 1.10.223 that is included with MPLAB X IDE v6.00)
- XC16 `v2.10`

# Current projects

> IMPORTANT!
>
> When using MPLAB X IDE v6.10 - Plugins must be Updated to latest version, otherwise
> MPLAB will not find PKOB (PICkit on Board) Programmer/Debugger on Curiosity board!
>
> To quote https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide Jul 24, 2023:
>
> > For MPLAB X IDE v6.10, install the five plug-ins that are suggested in the popup notification.
> > These plug-ins will enable the MPLAB PICkit 3 and MPLAB ICD 3 In-Circuit Debuggers/Programmers
> > and onboard debugging for certain evaluation board boards and fix connectivity issues.

* [ADC1.x](ADC1.x) - real time sampling signal using ADC on AN14/RC2 and outputing
  it on DAC/RB11)

![ADC DAC Quantization Digilent](assets/adc-dac-quantization.png)

You can download Digilent Analog Discovery workspace here:

- [assets/curiosity-adc.dwf3work](assets/curiosity-adc.dwf3work)


Required wiring:

| Digilent AD2 scope | dsPIC33CH Curiosity board |
| --- | --- |
| GND | GND - J11 pin 35 |
| W1 (Wavegen 1) | AN14/RC2  - J12 pin 3 |
| CH1 (Channel 1) | AN14/RC2 - J12 pin 3 |
| CH2 (Channel 2) | RB2/DACOUT1 - J12 pin 13 |
| Digital 0 | RB11/ADC_DONE - J11 pin 24
| Digital 1 | RB12/CCP1A - J11 pin 22 |

Additionally connect UART port to your PC (with micro-USB cable) and
in Putty set:
- Serial - your COMx port
- Speed: 115200
- format: 8-bit data, no  parity, no control flow

More details:
  - now uses only master core at 180 Mhz / 90 MIPS
  - toggle LED1 with rate 100 ms from TMR1 interrupt
  - toggle LED2 from main loop with 1s rate.
  - ADC input uses AN14/RC2 - put some signal there, limits are:
    - minimum Voltage: 0V
    - maximum Voltage: 3.3V (same as Vdd)
    - ADC is 12-bit, range 0 to 4095 (0xfff)
  - DAC Output on DACOUT1/RB2 - copies data from input in real-time (every 40.53 µs),
    unfortunately it is allowed to use only range 205 to 3890 instead of 0 to 4095 (see datasheet) - so 
    output amplitude is a bit smaller.
  - RB11 toggle on every finished ADC conversion
  - RB12 CCP Timer - toggles on every CCP event, triggers ADC Conversion on TMR overflow
  - currently sample rate is 40.53 µs (defined by CCP1 period, that triggers ADC conversion)
  - currently 1 ADC conversion takes 1.2738 µs (measured on scope - there is possible error)
  - UART, output ADC stats every second, set: 115200 bps, 8 bit data, 1 stop, no parity, no flow


Example UART output (every second):

```
TICK=30901 last=3776 (3.177 V) min=201 (0.161 V) max=3958 (3.188 V) dac=3753
```

How it works:

- CCP1 Timer triggers ADC Conversion
- when ADC Conversion is finished it triggers ADC Interrupt
- in Interrupt we store current ADC value and update statistics and finally
  we output value to DAC output

Additionally:
- LED1 blinks when TMR1 interrupt occurs
- LED2 blinks every second from main loop. It is very important to verify that main
  program flow works! (There was bug in old version of code, where ADC interrupt immediatelly
  triggered another interrupt so main code stopped executing!)
- ADC stats are reported to UART (and reset) every 1s from main program loop.


