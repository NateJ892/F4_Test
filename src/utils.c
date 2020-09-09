#include "utils.h"

void Delay_ms(int ms)
{
	uint32_t time = ticks+ms;

	while (ticks <= time);
}
