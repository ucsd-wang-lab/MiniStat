/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef DAC_H
#define DAC_H

#include "main.h"
    
#define AD5693R_ADDR        0x4C    //7 bit, 8 bit 0x98
#define DAC_REF             2500 //dac reference in mV
#define MAX_DAC_POTENTIAL   (2*DAC_REF) //max possible potential from DAC

typedef enum {  DAC_CH_A = 0,
                DAC_CH_B = 1,
                DAC_CH_BOTH = 7,
    } dacChannel_t;
typedef enum { REF_OFF, REF_ON, } dacRef_t;
    
void dacSet(uint16_t mVolts, dacChannel_t ch, uint8_t posNum);
void dacSetRef(dacRef_t status);

#endif /* DAC_H */ 
/* [] END OF FILE */
