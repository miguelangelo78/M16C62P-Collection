#include "sfr62p.h"

void main(void) {
	/* Set LED's pins as output: */
	pd5 = 0xFF;
	p5 = 0;
	
	/* Switches' pins as input: */
	pd0 = 0;

	for(;;) {
		/* Press any switch and the leds will turn on */
		p5 = ~p0;	
	}
}
