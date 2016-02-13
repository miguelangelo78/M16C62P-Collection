#include "../../globals.h"

const unsigned char temp_lookup[] = 
{0,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,13,14,14,14,
 15,15,16,16,16,17,17,17,18,18,18,19,19,19,19,20,20,20,21,21,21,22,22,22,
 22,23,23,23,23,24,24,24,24,25,25,25,25,26,26,26,26,27,27,27,27,28,28,28,
 28,29,29,29,29,29,30,30,30,30,31,31,31,31,32,32,32,32,32,33,33,33,33,33,
 34,34,34,34,34,34,35,35,35,35,35,36,36,36,36,36,37,37,37,37,37,38,38,38,
 38,38,38,39,39,39,39,39,39,40};

thermistor_t temp = 0;
unsigned int old_adc = 0;

void thermistor_update(unsigned int adc_value) {
	if(adc_value==old_adc) return;
	
	old_adc = adc_value;
	temp = temp_lookup[994 - adc_value];
}

void thermistor_timer_callback(void) {
	ADC_RESTART();	
}

void thermistor_adc_callback(void) {
	thermistor_update(adc_read(6));
}

void thermistor_init(void) {
	adc_init(thermistor_adc_callback, 6, ADC_MODE_ONESHOT, 1, 1, ADC_DEFAULT);
	timer_init(thermistor_timer_callback, TIMERA1, 100, TIMER_MODE_NORMAL, 0, TIMER_CLK_F32, TIMER_PRI_3);
}

void thermistor_deinit(void) {
	adc_deinit();
	timer_deinit(TIMERA1);
}

thermistor_t thermistor_read(void) {
	return temp;
}
