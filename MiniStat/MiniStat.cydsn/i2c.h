/* ========================================
 *
 * Copyright Clarity Design, Inc., 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Clarity Design, Inc.
 *
 * ========================================
*/
#ifndef I2C_H_
#define I2C_H_

#include "main.h"

/***************************************
*         Function Prototypes
****************************************/
extern void I2CReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *value);
extern void I2CReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *value);
extern void I2CReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *value);
extern uint32 I2CReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *value);
extern void I2CWriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t value);
extern void I2CWriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t value);
extern void I2CWriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t value);
extern uint32 I2CWriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *value);
extern void I2CWriteWord(uint8_t devAddr, uint8_t regAddr, uint16_t value);
extern void I2CWriteWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *value);
extern uint32 I2CWriteCommand(uint8_t devAddr, uint8_t command);

uint8_t buffer[22];

/***************************************
*            Constants
****************************************/

/* Buffer and packet size */
#define BUFFER_SIZE     (3)
#define PACKET_SIZE     (BUFFER_SIZE)

/* Packet positions */
#define PACKET_SOP_POS  (0)
#define PACKET_CMD_POS  (1)
#define PACKET_STS_POS  (PACKET_CMD_POS)
#define PACKET_EOP_POS  (2)

/* Start and end of packet markers */
#define PACKET_SOP      (0x01u)
#define PACKET_EOP      (0x17u)

/* Command valid status */
#define STS_CMD_DONE    (0x00u)
#define STS_CMD_FAIL    (0xFFu)

/* Command valid status */
#define TRANSFER_CMPLT    (0x00u)
#define TRANSFER_ERROR    (0xFFu)

#define I2C_TIMEOUT_MS  (50)

#endif /* I2C_H */
/* [] END OF FILE */
