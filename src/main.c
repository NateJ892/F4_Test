#include "stm32f401xe.h"

#define LED_GPIO        GPIOA
#define LED_PIN         5


static void delay(unsigned time) {
    for (unsigned i=0; i<time; i++)
        for (volatile unsigned j=0; j<20000; j++);
}

int main(void) 
{
    RCC->AHB1ENR  |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN); //Enable Clock To GPIOA

    //GPIO Setup (PC13 Button, PA5 LED)
    GPIOA->MODER |= (GPIO_MODER_MODE5_0);
    //Button configured as Input by default
    
    while (1)
    {
	    if (GPIOC->IDR & GPIO_IDR_ID13_Msk)
	    {
		    GPIOA->BSRR |= GPIO_BSRR_BS5;
	    }
	    else
	    {
		    GPIOA->BSRR |= GPIO_BSRR_BR5;
	    }
    }
}

