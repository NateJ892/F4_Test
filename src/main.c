#include "stm32f401xe.h"
#include "core_cm4.h"
#include "i2c.h"

volatile uint32_t ticks = 0;
static volatile uint8_t addr = 0;

static void enable_SysTick(void);

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
    
    I2C_Init();
    I2C_Start(0x68<<1);
    I2C_Write(0x74);
    I2C_Stop();
    I2C_Start((0x68<<1) | 1);
    I2C_Read(&addr, 1);

    if ((addr>>1) & 0x68)
    {
	    while (1)
	    {
		    GPIOA->ODR ^= 1<<5;
		    Delay_ms(1000);
	    }
    }
    else
    {
	    while (1);
    }
}

static void enable_SysTick(void)
{
	SysTick->LOAD = (uint32_t) ((84000000/1000)-1);
	NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
	SysTick->VAL = 0UL;
	SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk );
	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void) //Interrupt Routine
{
	ticks++;
	GPIOA->ODR ^= GPIO_ODR_OD5;
}

