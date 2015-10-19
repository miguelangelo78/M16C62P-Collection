#include "delay.h"

void main(void) {
	pd5 = 0xFF;
	
	/* Blink all leds: */
	for(;;) {
		p5 = 0xFF;
		delay_ms(50);
		p5 = 0;
		delay_ms(50);
	}
}
