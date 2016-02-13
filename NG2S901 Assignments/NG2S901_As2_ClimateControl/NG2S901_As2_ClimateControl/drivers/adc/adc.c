#include "../../globals.h"

adc_cback_t adc_cback;

void adc_init_easy(adc_cback_t adc_callback, char channel) {
	adc_cback = adc_callback;
	
	/* Configure ADCON0: */
	ADCON0 = (channel & 0x07);
	
	/* Configure everything else with default settings: */
	ADCON1 = 0x28;
	ADCON2 = 0x05;
	if(adc_callback) { /* If callback is null don't even bother about the ISR */
		disable_interrupt();
		ADIC = 0x04;
		enable_interrupt();
	}
	ADST = 0x01;
}

void adc_init(adc_cback_t adc_callback, char channel, char adc_mode, char tenbits_size, char sampleandhold, int portsel) {
	adc_cback = adc_callback;
	
	/* Configure ADCON0: */
	ADCON0 = (channel & 0x07) | (adc_mode & 0x03) << 0x03; /* Channel and mode */
	
	/* Configure ADCON1: */
	ADCON1 = (tenbits_size & 0x01) << 0x03 | 0x20; /* Also connect VRef (thus 0x20) */
	
	/* Configure ADCON2: */
	ADCON2 = (sampleandhold & 0x01);
	if(portsel == ADC_DEFAULT)
		ADCON2 |= 0x04; /* Default configuration in case portsel is unimportant */
	else
		ADCON2 |= (portsel & 0x03) << 0x01;
		
	/* Configure priority: */
	if(adc_callback) { /* If callback is null don't even bother about the ISR */
		disable_interrupt();
		ADIC = 0x04;
		enable_interrupt();
	}
	
	/* Start up ADC: */
	ADST = 0x01;
}

void adc_deinit(void) {
	/* Stop channel and mode: */
	ADCON0 = 0x0;
	/* Stop everything else related to ADCs: */
	ADCON1 = 0x0;
	ADCON2 = 0x0;
	disable_interrupt();
	ADIC = 0x0;
	enable_interrupt();
}

unsigned int adc_read(char channel) {
	int * adc_register;
	if(channel >= 8 || channel < 0) return 0; /* Invalid register index */

	/* Grab value from AD0, or AD1,..., AD7, by using a pointer: */	
	adc_register = (int*)((&AD0_ADDR) + channel);
	return *adc_register;
}

#pragma interrupt ADC_Interrupt
void ADC_Interrupt(void) {
	if(adc_cback) adc_cback();
}