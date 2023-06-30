/**
 ADC related example for dsPIC33CH on Curiosity board

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

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
#include <libpic30.h> 
/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"

// Timer handler called with 100 ms period - must have this name
// to override default callback in tmr1.c
void  TMR1_CallBack(void)
{
    // LED1 blinking period = 2 * TIMER_PERIOD ms = 200 ms
    RE0_LED1_Toggle();
}


ADC1_CHANNEL channel = AN14_ADC;
uint16_t lastVal = 0;
uint16_t minVal  = ~0;
uint16_t maxVal = 0;

void ADC1_AN14_Handler(uint16_t adcVal)
{
    lastVal = adcVal;
    if (adcVal > maxVal)
        maxVal = adcVal;
    if (adcVal < minVal)
        minVal = adcVal;
    RB11_GPIO1_Toggle();
}


/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    RE0_LED1_SetLow();
    RB11_GPIO1_SetLow();
    ADC1_Enable();
    ADC1_ChannelSelect(channel);
    TMR1_Start();
    ADC1_SetAN14_ADCInterruptHandler(ADC1_AN14_Handler);
    // AN14 Level Trigger must be enabled
    ADLVLTRGLbits.LVLEN13 = 1;
    // enable continuous ADC Triggers
    ADCON3Lbits.SWLCTRG = 1;
    while (1)
    {
        
        __delay_us(1);
        // Add your application code
    }
    return 1; 
}
/**
 End of File
*/
