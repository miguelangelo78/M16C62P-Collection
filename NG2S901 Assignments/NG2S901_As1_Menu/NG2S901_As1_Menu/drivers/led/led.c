#include "led.h"
#include "../init/sfr62p.h"
#include "../keypad/keyscan_4x4.h"

/******************* LED PATTERNS: ********************/
int shift = 0;
char shift_dir = 1;
unsigned int shift_delay = 0;
unsigned int shift_delay_max = 700;

unsigned int dim_delay = 0;
unsigned int dim_brightness = DIM_BRIGHTNESS_MAX / 2;

unsigned int countup_delay = 0;
unsigned int countup_delay_max = 700;

void led_pattern_update(char patt_type) {
	switch(patt_type) {
	case LED_PATT_SHIFTSIDES: /* Shift leds sideways */
		if(shift_delay++ < shift_delay_max) return;
		shift_delay = 0;
		if(shift_dir) {
			P5 = (1 << shift++);
			if(shift >= 8)
				shift_dir = !shift_dir;
		} else {
			P5 = (1 << shift--);
			if(shift < 0)
				shift_dir = !shift_dir;
		}
		break;
	case LED_PATT_DIM_ALL: /* Dims all leds at the same time */
		if(dim_delay > DIM_DELAY_MAX/2 - dim_brightness && dim_delay < DIM_DELAY_MAX/2 + dim_brightness) 
			P5 = 0xFF;
		else 
			P5 = 0x00;	
		if(dim_delay++ > DIM_DELAY_MAX)
			dim_delay = 0;
		break;
	case LED_PATT_COUNTUP: /* Counts up the LEDs in binary */
		if(countup_delay++ > countup_delay_max) {
			countup_delay = 0;
			P5++;
		}
		break;
	}
	
	/* Increase/decrease shift speed: */
	switch(getcommand()) {
	case '1': 
		if((shift_delay_max += 100) > 20000) shift_delay_max = 20000;
		break;
	case '2':
		if((shift_delay_max -= 100) < 500) shift_delay_max = 500;
		break;
	}
}
