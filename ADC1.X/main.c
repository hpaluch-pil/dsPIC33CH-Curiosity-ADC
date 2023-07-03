/**
 ADC related example for dsPIC33CH on Curiosity board

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    ADC1 example for dsPIC33CH.
    Used peripherals:
    - AN14/RC2 - ADC input. Allowed voltage: 0V to 3.3V (=Vdd)
    - RB11 GPIO Out - toggled on every ADC interrupt (finished acquisition)
    - RE0 LED1 - toggled every 100 ms - so blinks with 200 ms rate.
    - RE1 LED2 - toggle every 1s from main loop
    - RC11 TXB - UART (115200 bps, 8 data, 1 stop, no parity, no flow)
               - ADC stats every 1s
    Notes:
    - currently 1 ADC conversion takes 1.69 µs
  
  
  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.2
        Device            :  dsPIC33CH512MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05 (using v6.00 - see README.md)
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

// must define instruction clock for Master
// f_osc = 180 MHz, f_cy = 90 MHz => 90 MIPS
#define FCY 90000000UL 
#include <libpic30.h> // __delay_xx())
#include <stdio.h> // printf(3)
/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"


#define APP_VERSION 100 // = 1.00

// counter increased every 100ms from interrupt
volatile uint16_t tmr1_counter = 0;

// Timer handler called with 100 ms period - must have this name
// to override default callback in tmr1.c
void  TMR1_CallBack(void)
{
    tmr1_counter++;
    // LED1 blinking period = 2 * TIMER_PERIOD ms = 200 ms
    RE0_LED1_Toggle();
}


ADC1_CHANNEL channel = AN14_ADC;
volatile uint16_t lastVal = 0;
volatile uint16_t minVal  = ~0;
volatile uint16_t maxVal = 0;

// overrides interrupt elsewhere
void __attribute__ ( ( __interrupt__ , auto_psv) ) _ADCAN14Interrupt ( void )
{
    uint16_t adcVal= ADCBUF14;
    
    if (adcVal > maxVal)
        maxVal = adcVal;
    if (adcVal < minVal)
        minVal = adcVal;
    RB11_GPIO1_Toggle();

    //clear the AN14_ADC interrupt flag
    IFS6bits.ADCAN14IF = 0;
    // starts another conversion, should be after IF flag clearing....
    //ADC1_SoftwareTriggerEnable();
    //ADCON3Lbits.SWCTRG = 1;
}


/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    RE0_LED1_SetLow();
    RE1_LED2_SetLow();
    RB11_GPIO1_SetLow();
    ADC1_Enable();
    ADC1_ChannelSelect(channel);
    TMR1_Start();
    printf("MASTER: at %s() %s:%d v%d.02%d started\r\n",
            __func__,__FILE__,__LINE__,APP_VERSION/100,APP_VERSION%100);
    // will call interrupt on completion...
    ADC1_SoftwareTriggerEnable();
    while (1)
    {
        // ACK on LED2 that while() loop is working properly...
        RE1_LED2_Toggle();
        // wait 1s
        while( (tmr1_counter % 10) != 0); /*NOP*/
        // avoid re-trigger....
        while( (tmr1_counter % 10) == 0); /*NOP*/
        // print info on sampled ADC value every 1 second...
        printf("TICK=%u last=%u min=%u max=%u\r\n",
                tmr1_counter, lastVal, minVal, maxVal);
    }
    return 1; 
}
/**
 End of File
*/
