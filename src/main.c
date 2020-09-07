#include "stm32f401xe.h"
#include <math.h>
#include "core_cm4.h"

static volatile uint32_t ticks = 0;
static volatile uint8_t addr = 0;

static void enable_SysTick(void);
static void Delay_ms(int ms);

int main(void) 
{
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN); //Enable Clock To GPIOA, B, and C
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //Enable I2C1 Clock

    //GPIO Setup (PC13 Button, PA5 LED)
    GPIOA->MODER |= (GPIO_MODER_MODE5_0);
    //Button configured as Input by default
    //Clock Is auto configured to 84Mhz for us
    
    //I2C Pin Setup (PB8 == SCL, PB9 == SDA)
    GPIOB->MODER  |= (GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
    GPIOB->OTYPER |= (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
    GPIOB->AFR[1] |= ((4 << GPIO_AFRH_AFSEL8_Pos) | (4 << GPIO_AFRH_AFSEL9_Pos));

    __enable_irq();
    enable_SysTick();
    
    //I2C Task    
    I2C1->CR1   |= I2C_CR1_SWRST;
    Delay_ms(5);
    I2C1->CR1   &= ~I2C_CR1_SWRST;

    I2C1->CR2   |= 42;
    I2C1->TRISE |= 43;
    I2C1->CCR   |= 180;
    I2C1->OAR1  |= (1<<14);
    I2C1->CR1   |= I2C_CR1_PE;

    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0x68<<1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    (void)I2C1->SR2;

    I2C1->DR = 0x75;
    while (!(I2C1->SR1 & I2C_SR1_BTF));

    I2C1->CR1 |= I2C_CR1_STOP;
    while (I2C1->SR2 & I2C_SR2_BUSY);

    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = (0x68<<1 | 1);
    while (!(I2C1->SR1 * I2C_SR1_ADDR));
    (void)I2C1->SR2;
    while (!(I2C1->SR1 & I2C_SR1_RXNE));

    addr = (uint8_t)I2C1->DR;

    I2C1->CR1 |= I2C_CR1_STOP;
    while (I2C1->SR2 & I2C_SR2_BUSY);

    while (1);
}

static void enable_SysTick(void)
{
	SysTick->LOAD = (uint32_t) ((84000000/1000)-1);
	NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
	SysTick->VAL = 0UL;
	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk );
	NVIC_EnableIRQ(SysTick_IRQn);
}
	
static void Delay_ms(int ms)
{
	uint32_t time = ticks+ms;

	while (ticks <= time);
}

void SysTick_Handler(void) //Interrupt Routine
{
	ticks++;
	GPIOA->ODR ^= GPIO_ODR_OD5;
}

