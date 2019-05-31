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
#include "main.h"
#include "i2c.h"

uint32 I2CReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *value) {
	uint8_t i=0;
	I2CM_I2CMasterSendStart(devAddr, I2CM_I2C_WRITE_XFER_MODE, I2C_TIMEOUT_MS);
	I2CM_I2CMasterWriteByte(regAddr, I2C_TIMEOUT_MS);
    I2CM_I2CMasterSendRestart(devAddr, I2CM_I2C_READ_XFER_MODE, I2C_TIMEOUT_MS);
	while (i++ < (length-1)) {
		I2CM_I2CMasterReadByte(I2CM_I2C_ACK_DATA, value++, I2C_TIMEOUT_MS);
	}
	I2CM_I2CMasterReadByte(I2CM_I2C_NAK_DATA, value, I2C_TIMEOUT_MS);
	return I2CM_I2CMasterSendStop(I2C_TIMEOUT_MS);	
}

void I2CReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *value) {
	I2CReadBytes(devAddr, regAddr, 1, value);
}

void I2CReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *value) {
   	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    I2CReadByte(devAddr, regAddr, value);
    *value &= mask;
    *value >>= (bitStart - length + 1);
}

void I2CReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *value) {
	I2CReadByte(devAddr, regAddr, value);
	*value = *value & (1 << bitNum);
}

uint32 I2CWriteCommand(uint8_t devAddr, uint8_t command) {
	I2CM_I2CMasterSendStart(devAddr, I2CM_I2C_WRITE_XFER_MODE, I2C_TIMEOUT_MS);
	I2CM_I2CMasterWriteByte(command, I2C_TIMEOUT_MS);
	return I2CM_I2CMasterSendStop(I2C_TIMEOUT_MS);	
}

uint32 I2CWriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *value) {
	uint8_t i=0;
	I2CM_I2CMasterSendStart(devAddr, I2CM_I2C_WRITE_XFER_MODE, I2C_TIMEOUT_MS);
	I2CM_I2CMasterWriteByte(regAddr, I2C_TIMEOUT_MS);
	while (i++ < length) {
		I2CM_I2CMasterWriteByte(*value++, I2C_TIMEOUT_MS);
	}
	return I2CM_I2CMasterSendStop(I2C_TIMEOUT_MS);	
}

void I2CWriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t value) {
	I2CWriteBytes(devAddr, regAddr, 1, &value);
}

void I2CWriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t value) {
	uint8_t b;
	uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
	I2CReadByte(devAddr, regAddr, &b);
	value <<= (bitStart - length + 1);
	value &= mask;
	b &= ~(mask);
	b |= value;
	I2CWriteByte(devAddr, regAddr, b);	
}

void I2CWriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t value) {
	uint8_t b;
	I2CReadByte(devAddr, regAddr, &b);
	b = (value != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	I2CWriteByte(devAddr, regAddr, b);
}

void I2CWriteWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *value) {
	uint8_t i=0;
	I2CM_I2CMasterSendStart(devAddr, I2CM_I2C_WRITE_XFER_MODE, I2C_TIMEOUT_MS);
	I2CM_I2CMasterWriteByte(regAddr, I2C_TIMEOUT_MS);
	while (i++ < length) {
		I2CM_I2CMasterWriteByte(((uint8_t)*value) >> 8, I2C_TIMEOUT_MS);
		I2CM_I2CMasterWriteByte((uint8_t)*value++, I2C_TIMEOUT_MS);
	}
	I2CM_I2CMasterSendStop(I2C_TIMEOUT_MS);		
}

void I2CWriteWord(uint8_t devAddr, uint8_t regAddr, uint16_t value) {
	I2CWriteWords(devAddr, regAddr, 1, &value);
}

/* [] END OF FILE */
