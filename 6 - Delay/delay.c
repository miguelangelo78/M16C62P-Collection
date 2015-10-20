#include "sfr62p.h"
#include "delay.h"
#include "cpufreq.h"

void main(void) {
	cpu_init();
	pd5 = 0xFF;
	
	/* Blink all leds: */
	for(;;) {
		p5 = 0xFF;
		delay_ms(1000);
		p5 = 0;
		delay_ms(1000);
	}
}
