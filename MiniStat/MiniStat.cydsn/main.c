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

#include <project.h>
#include "main.h"

/*Function declarations */
uint32_t getTick(void);
void resetTick(void);
void tickStart(void);
void tickStop(void);
void sysManagePower(void);

/* Global variables*/
volatile uint32_t tickTimer = 0;
uint8_t sysHibernate = FALSE;
/*******************************************************************************
* Function Name: Tick_Interrupt
********************************************************************************
*
* Summary:
*  Handles the Interrupt Service Routine for the WDT tick mechanism.
*
*******************************************************************************/
CY_ISR(Tick_Interrupt) // MAIN_TICK_FREQ
{
    /* Indicate that timer is raised to the main loop */
    tickTimer++;
}

uint32_t getTick(void)
{
//    #if (DEBUG_UART_ENABLED == ENABLED)
//    if(tickTimer > 1)
//    {
//        //DBG_PRINTF("**********\tMissed tick! Tick Val: %d\t**********\r\n", tickTimer);
//    }
//    #endif
    return tickTimer;
}

void resetTick(void)
{
    tickTimer = 0;
    return;
}

void tickStart(void)
{
    /* Unlock the WDT registers for modification */
    CySysWdtUnlock(); 
    // Register Tick_Interrupt() by the WDT COUNTER0 to generate an interrupt every tick
    CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER0, Tick_Interrupt);
    // Enable the Counter0 ISR Handler
    CySysWdtEnableCounterIsr(CY_SYS_WDT_COUNTER0);
    /* Lock out configuration changes to the Watchdog timer registers */
    CySysWdtLock();   
}

void tickStop(void)
{
    /* Unlock the WDT registers for modification */
    CySysWdtUnlock(); 
    /* Disable the specified WDT counter */
    CySysWdtDisable(CY_SYS_WDT_COUNTER0);
    /* Locks out configuration changes to the Watchdog timer registers */
    CySysWdtLock();    
}

#define TEMP_TOGGLE_CNT     4096
uint16_t temp_cnt = 0;

int main (void)
{
    CyGlobalIntEnable;                // Enable global interrupts
    tickStart();
    
    //lmp_setup(); // Configures LMP91000 through I2C 
    //LED_2_Write(GPIO_HIGH);    
    while(1)
   {
        // TODO : Read AFE_OUT here
        if (getTick())
        {
            if (++temp_cnt == TEMP_TOGGLE_CNT)
           { 
                temp_cnt = 0;
                (LED_1_Read() == GPIO_HIGH) ? LED_1_Write(GPIO_LOW) : LED_1_Write(GPIO_HIGH);
                //(LED_2_Read() == GPIO_HIGH) ? LED_2_Write(GPIO_LOW) : LED_2_Write(GPIO_HIGH);
            }
            resetTick();            
       } 
    }
}

/*This function name cannot change.*/
void CyBoot_IntDefaultHandler_Exception_EntryCallback(void)
{
    //Turn on all LEDs, wait, then reset
    LED_1_Write(GPIO_HIGH);
    LED_2_Write(GPIO_HIGH);
    CyDelay(2000);
    CySoftwareReset();
}