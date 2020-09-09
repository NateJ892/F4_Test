#include "i2c.h"

void I2C_Init(void)
{
    I2C1->CR1 |= I2C_CR1_SWRST;
    Delay_ms(5);
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    I2C1->CR2   |= 42;
    I2C1->TRISE |= 43;
    I2C1->CCR   |= 180;
    I2C1->OAR1  |= (1<<14);
    I2C1->CR1   |= I2C_CR1_PE;
}

void I2C_DeInit(void)
{
    I2C1->CR1 &= ~I2C_CR1_PE;
}

void I2C_Start(uint8_t addr)
{
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = addr;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2;
}

void I2C_Stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
    while (I2C1->SR2 & I2C_SR2_BUSY);
}

void I2C_WriteBuffer(uint8_t *buf, uint32_t nbytes)
{
    while (nbytes > 1)
    {
	    I2C1->DR = *buf;
	    buf++;
	    nbytes--;
	    while (!(I2C1->SR1 & I2C_SR1_TXE));
    }

    I2C1->DR = *buf;
    while (!(I2C1->SR1 & I2C_SR1_BTF));
}   

void I2C_Write(uint8_t byte)
{
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1->DR = byte;

    while (!(I2C1->SR1 & I2C_SR1_BTF));
}

void I2C_Read(uint8_t *buf, uint32_t nbytes)
{
    while (nbytes > 1)
    {
        I2C1->CR1 |= I2C_CR1_ACK;

	while (!(I2C1->SR1 & I2C_SR1_RXNE));

	*buf = I2C1->DR;
	buf++;
	nbytes--;
    }

    I2C1->CR1 &= ~I2C_CR1_ACK;

    while (!(I2C1->SR1 & I2C_SR1_RXNE));
    *buf = I2C1->DR;
    //while (!(I2C1->SR1 & I2C_SR1_BTF));
}

