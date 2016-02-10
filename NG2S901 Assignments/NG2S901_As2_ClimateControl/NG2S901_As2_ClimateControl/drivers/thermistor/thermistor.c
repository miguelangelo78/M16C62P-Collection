#include "../../globals.h"

thermistor_t temp;

void thermistor_update(unsigned int adc_value) {
	temp = adc_value;
}

void thermistor_timer_callback(void) {
	ADC_RESTART();	
}

void thermistor_adc_callback(void) {
	thermistor_update(adc_read(6));
}

void thermistor_init(void) {
	adc_init(thermistor_adc_callback, ADC1 | ADC2, ADC_MODE_ONESHOT, 0, 1, ADC_DEFAULT);
	timer_init(thermistor_timer_callback, TIMERA1, 4, TIMER_MODE_NORMAL, TIMER_CLK_F32, TIMER_PRI_1);
}

void thermistor_deinit(void) {
	adc_deinit_all();
	timer_deinit(TIMERA1);
}

thermistor_t thermistor_read(void) {
	return temp;
}
