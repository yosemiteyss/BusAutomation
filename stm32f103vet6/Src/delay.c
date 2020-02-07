#include "delay.h"

void delay_us ( uint16_t duration ) 
{
	while (duration--) {
		uint16_t i = 0x06;
			
		while (i--)
				__asm("nop");
	}
}
