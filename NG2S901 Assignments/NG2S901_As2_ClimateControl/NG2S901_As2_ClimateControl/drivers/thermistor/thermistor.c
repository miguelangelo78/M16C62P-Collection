#include "../../globals.h"

const unsigned char temp_lookup[] = 
{0,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,13,14,14,14,
 15,15,16,16,16,17,17,17,18,18,18,19,19,19,19,20,20,20,21,21,21,22,22,22,
 22,23,23,23,23,24,24,24,24,25,25,25,25,26,26,26,26,27,27,27,27,28,28,28,
 28,29,29,29,29,29,30,30,30,30,31,31,31,31,32,32,32,32,32,33,33,33,33,33,
 34,34,34,34,34,34,35,35,35,35,35,36,36,36,36,36,37,37,37,37,37,38,38,38,
 38,38,38,39,39,39,39,39,39,40};

char current_thermistor;

void thermistor_init(void) {
	current_thermistor = eeprom_read(1);
	adc_init(0, 6, ADC_MODE_REPEAT, 1, 1, ADC_DEFAULT);
	i2c_thermistor_init();
}

void thermistor_deinit(void) {
	adc_deinit();
	i2c_thermistor_deinit();
}

void thermistor_toggle(char thermistor_type) {
	current_thermistor = thermistor_type & 0x1;
	eeprom_write(1, current_thermistor);
	if(current_thermistor == THERMISTOR_DIGITAL) /* If we're using the digital thermistor, turn off ADC */
		adc_deinit();
	else /* Otherwise turn it back on */
		adc_init(0, 6, ADC_MODE_REPEAT, 1, 1, ADC_DEFAULT);
}

thermistor_t thermistor_read(void) {
	if(current_thermistor == THERMISTOR_ANALOGUE)
		return temp_lookup[994 - adc_read(6)];
	else
		i2c_thermistor_read_temp();
}

/* Are we using analogue or digital thermistor: */
char thermistor_type(void) {
	return current_thermistor;	
}