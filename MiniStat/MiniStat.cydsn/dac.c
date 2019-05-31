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
#include "main.h"
#include "i2c.h"
#include "dac.h"

/* DAC is AD5647R*/
#define DAC_LVLS        16384   //2^14
#define DAC_SHIFT       9214    //1.406v -> dac code

uint16_t dacVolts2Code(uint16_t mVolts, uint8_t posNum);

uint16_t dacVolts2Code(uint16_t mVolts, uint8_t posNum)
{
    uint16_t code = 0;
    //DBG_PRINTF("DAC mV: %d\r\n", mVolts);
    if (posNum == TRUE)
    {
        code = (mVolts * DAC_LVLS)/(2*DAC_REF) + DAC_SHIFT;
    }
    else
    {
        code = DAC_SHIFT - (mVolts * DAC_LVLS)/(2*DAC_REF);
    }
    if (code > DAC_LVLS)
    {
        DBG_PRINTF("ERROR! DAC out of range. Code: %d\r\n", code);
        code = 0;
    }
    return code;
}

void dacSet(uint16_t mVolts, dacChannel_t ch, uint8_t posNum)
{
    /* VOUT = 2*VREF*(val/(2^14))*/
    
    /* Command sequence, 3 bytes total
    //  Write Address with R/W = 0
    //  Write Command byte
    //      R -> reserved
    //      S -> Byte selection: 0=3 bytes, 1=2 bytes 
    //      C0, C1, C2  -> Commands
                0=write to input register n
                1=update dac register n
                2=write to input register n, update all (software LDAC)
                3=write to and update DAC channel n
                4=power up/power down
                5=reset
                6=LDAC register setup
                7=internal reference on/off
    //      A1, A2, A3  -> Address(n):0=CH_A, 1=DAC_B, 7=BOTH 
    //  Write MSB of data
    //  Write LSD of data
    */

    uint8_t cmd_byte = 0;//0001 1 A1A2A3
    switch(ch)
    {
        case DAC_CH_A:
            cmd_byte=0x18;
        break;
        case DAC_CH_B:
            cmd_byte=0x19;
        break;
        case DAC_CH_BOTH:
            cmd_byte=0x1F;
        break;
    }
    
    uint16_t val = dacVolts2Code(mVolts, posNum);   //convert to code
    //DBG_PRINTF("Set DAC Volts: %d\tCode: 0x%x\r\n", mVolts, val);
    val = val << 2; //Shift data 2 bits up since last 2 bits are unused
    uint8_t data[2]; 
    data[0] = (val >> 8);
    data[1] = val & 0xFF;
    //DBG_PRINTF("Set DAC val: 0x%x|0x%x|0x%x\r\n", cmd_byte, data[0], data[1]);
    I2CWriteBytes(AD5693R_ADDR, cmd_byte, 2, data);//regAddr is just the first byte written after device addr, used as cmd byte
}

void dacSetRef(dacRef_t status)
{
    #define REF_SETUP_CMD   0x38 //cmd: 0011 1000 -> 0x38
    uint8_t cmd_byte = REF_SETUP_CMD;    //reference setup command
    uint8_t data[2]; 
    data[0] = 0;
    data[1] = status & 0xFF;
    DBG_PRINTF("Set DAC Ref: 0x%x|0x%x|0x%x\r\n", cmd_byte, data[0], data[1]);
    I2CWriteBytes(AD5693R_ADDR, cmd_byte, 2, data);//regAddr is just the first byte written after device addr, used as cmd byte
}


/* [] END OF FILE */
