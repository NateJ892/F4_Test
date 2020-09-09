#ifndef I2C_LIB
#define I2C_LIB

#include "stm32f401xe.h"
#include "utils.h"

void I2C_Init(void);
void I2C_DeInit(void);

void I2C_Start(uint8_t addr);
void I2C_Stop(void);

void I2C_WriteBuffer(uint8_t *buf, uint32_t nbytes);
void I2C_Write(uint8_t byte);
void I2C_Read(uint8_t *buf, uint32_t nbytes);

#endif
