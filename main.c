/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F18313
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdint.h>

/*
                         Main application
 */

const uint16_t MAX_INPUT_VALUE = 80; // the actual sampled value when car is at full stop.
const uint16_t SPEED_CHANGE_RATE = 2046;
const uint32_t MIN_SPEED = 10; // in percent


uint16_t valIn = 0;
uint16_t valOut = 0;
uint32_t targetA = 0;
uint32_t targetB = 0;
uint32_t speedA = 0;
uint32_t speedB = 0;

uint32_t scale(uint32_t value){
    if (value > MAX_INPUT_VALUE) value = MAX_INPUT_VALUE;
    value = MAX_INPUT_VALUE - value; // 0 = full stop, MAX_INPUT_VALUE = full speed
    value = value * (1023000 / MAX_INPUT_VALUE); // 0=stop, 1023000=full speed
    return value;
}

uint32_t filter(uint32_t target, uint32_t speed){
    if (target > speed) speed += SPEED_CHANGE_RATE;
    else if (target < speed){
        if (SPEED_CHANGE_RATE >= speed) return 0;
        else speed -= SPEED_CHANGE_RATE;
    }
    return speed;
}

uint32_t curve(uint32_t value){
    value = value/10000; //max 10 230
    uint32_t val1 = value;
    uint32_t val2 = value;
    value = (val1 * val2);
    value = (value / 102);
//    value = (value*value)/1023; //max 102 300
    return (value*10000); //max 1 023 000
}

uint32_t min_adjust(uint32_t value){
    value = value / 100; //max 10 230
    if (value < 10) return 0;
    uint32_t dx = 10220;
    uint32_t dy=1023000000-(MIN_SPEED*(10230000)); //MIN_SPEED is in percent
    uint32_t kf = dy/dx;
    uint32_t mt = (MIN_SPEED*(10230000)) - (10*kf);
    value = (value*kf) + mt;
    return value/1000000; //max 1023
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    while (1)
    {
        valIn = ADC_GetConversion(AN_1);
        targetA = scale(valIn);
        speedA = filter(targetA, speedA);
//        speedA = curve(speedA);
//        speedA = min_adjust(speedA);
        PWM5_LoadDutyValue(speedA/1000);
        
        valIn = ADC_GetConversion(AN_2);
        targetB = scale(valIn);
        speedB = filter(targetB, speedB);
//        speedB = curve(speedB);
//        speedB = min_adjust(speedB);
        PWM6_LoadDutyValue(speedB/1000);
    }
}
/**
 End of File
*/