#include "stm32f401xe.h"
#include <math.h>
#include "core_cm4.h"

static volatile uint32_t ticks = 0;

static void enable_SysTick(void);

int main(void) 
{
    RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN); //Enable Clock To GPIOA

    //GPIO Setup (PC13 Button, PA5 LED)
    GPIOA->MODER |= (GPIO_MODER_MODE5_0);
    //Button configured as Input by default
    //Clock Is auto configured to 84Mhz for us
    __enable_irq();

    enable_SysTick();
    while (1)
    {
	    if (ticks >= 5000)	//Should Be 5 Seconds Before LED Turns On!
	    {
		    GPIOA->BSRR |= GPIO_BSRR_BS5;
	    }
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
