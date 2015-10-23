#include "sfr62p.h"
#include "cpufreq.h"
#include <string.h>

/* This file does PWM through the digital output port P5 without using timers */

#define bit_set(p,m) ((p) |= (1<<m))
#define bit_clear(p,m) ((p) &= ~(1<<m))
#define led_update() {int i; \
					for(i=0;i<8;i++) \
						pwm(i*32, 256-(i*32), i);}
int pwm_preload[8];

inline void pwm(int pulse_width, int period, int channel) {
	/* Set proper scale: */
	pulse_width /= 8;
	period *= 2;
	
	/* Period: */
	if(pulse_width && pwm_preload[channel] <= period * -1) {
		pwm_preload[channel] = pulse_width;
		bit_set(p5, channel);
	}
	
	/* Pulse width: */
	if(!(pwm_preload[channel]--))
		bit_clear(p5, channel);	
}

void main(void) {
	cpu_init();
	
	pd5 = 0xFF;	
	for(;;) led_update();
}
