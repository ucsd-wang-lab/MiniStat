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
#ifndef BLEHAND_H
#define BLEHAND_H

#include "main.h"
    
/*General*/  
#define BLE_ADVERT_LENGTH   (NAME_LENGTH + 1 + CHIP_ID_LENGTH)    
#define CHIP_ID_LENGTH      (16)  // 8 bytes in 16 hexadecimal digits for 128 bit chip identifier
#define ERR_INVALID_PDU     0x04
#define CCCD_VALID_BIT_MASK 0x03
#define NOTIFY_BIT_MASK     0x01
    
#define MINIMUM_CONNECTION_INTERVAL                 (790u) /* 790 * 1.25 = 987.5ms */
#define MAXIMUM_CONNECTION_INTERVAL                 (800u) /* 800 * 1.25 = 1000ms */
#define SLAVE_LATENCY                               (0u)   /* No slave latency */
#define SUPERVISION_TIMEOUT                         (500u) /* Supervision timeout of 5 seconds */       
    

enum {
    BLE_ST_INIT = 0,
    BLE_ST_IDLE,
    BLE_ST_ACTIVE,
    BLE_ST_COUNT
};

/*Getters/Setters for BLE data*/

uint8_t getBleHibernateFlag(void);
void setBleHibernateFlag(uint8_t flag);
uint8_t getMcuHibernateFlag(void);
uint8 getBleConnection(void);

/*BLE Functions*/
CYBLE_API_RESULT_T bleStart(void);
void bleStop(void);
void bleHibernate(void);
void bleManagePower(void);
void bleWake(void);

CYBLE_API_RESULT_T GetAPI_Result(void);
void SetAPI_Result(CYBLE_API_RESULT_T api);

#endif

/* [] END OF FILE */
