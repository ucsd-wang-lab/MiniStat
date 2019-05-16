/* ========================================
 *
 * NANOBIOELECTRONICS LAB UCSD, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF the company.
 *
 * ========================================
 */

/** Note that the file assumes I2C_Start() has been called by main**/

#include "amperometry_api.h"

void lmp_setup()
{
    uint8 lmpModeData[] = {LMP_REG_MODECN_ADDR, LMP_3_LEAD_AMPEROMETRIC_CELL_MODE};
    uint8 lmpRefData[]  = {LMP_REG_REFCN_ADDR, LMP_INT_BIAS_DEFAULT};
    uint8 lmpTiaData[]  = {LMP_REG_TIACN_ADDR, LMP_TIA_DEFAULT};
    uint32 lmpDataCount = 2;
    uint8 lmpXferInitiated = 0;
    uint8 lmpXferFinished = 0;
    
    while(lmpXferFinished != 3)
    {   
        if(lmpXferInitiated == 0)
        {
            I2C_I2CMasterWriteBuf(LMP_ADDR, lmpModeData, lmpDataCount, 
                                  I2C_I2C_MODE_COMPLETE_XFER);
            lmpXferInitiated = 1;
        }
        else if(lmpXferFinished == 1 && lmpXferInitiated == 1)
        {
            I2C_I2CMasterWriteBuf(LMP_ADDR, lmpRefData, lmpDataCount, 
                                  I2C_I2C_MODE_COMPLETE_XFER);
            lmpXferInitiated = 2;
        }
        else if(lmpXferFinished == 2 && lmpXferInitiated == 2)
        {
            I2C_I2CMasterWriteBuf(LMP_ADDR, lmpTiaData, lmpDataCount, 
                                  I2C_I2C_MODE_COMPLETE_XFER);
            lmpXferInitiated = 3;
        }
        else if(I2C_I2CMasterGetWriteBufSize() == lmpDataCount)
        {
            lmpXferFinished++;
        }
    }
}