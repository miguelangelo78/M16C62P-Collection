#include "../../globals.h"

adc_cback_t adc_cback;

void adc_init_easy(adc_cback_t adc_callback, char channels) {
	char channel_mask;
	adc_cback = adc_callback;
	
	/* Configure channels: */
	for(channel_mask = 0; channel_mask < ADC_CHANNEL_COUNT; channel_mask++) /* For every bit... */
		if(bit_get(channels, channel_mask)) /* Is bit set? */
			bit_set(ADCON0, channel_mask); /* Set that particular channel then */
	
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

void adc_init(adc_cback_t adc_callback, char channels, char adc_mode, char tenbits_size, char sampleandhold, int portsel) {
	char channel_mask;
	adc_cback = adc_callback;
	
	/* Configure channels: */
	for(channel_mask = 0; channel_mask < ADC_CHANNEL_COUNT; channel_mask++) /* For every bit... */
		if(bit_get(channels, channel_mask)) /* Is bit set? */
			bit_set(ADCON0, channel_mask); /* Set that particular channel then */
	
	/* Configure the rest of ADCON0: */
	ADCON0 |= (adc_mode & 0x03) << 0x03;
		
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

void adc_deinit(char channels) {
	char channel_mask;
	/* Clear channels: */
	for(channel_mask = 0; channel_mask < ADC_CHANNEL_COUNT; channel_mask++) /* For every bit... */
		if(bit_get(channels, channel_mask)) /* Is bit set? */
			bit_clear(ADCON0, channel_mask); /* UNSET that particular channel then */
}

void adc_deinit_all(void) {
	char i;
	/* Stop all channels: */
	ADCON0 = 0x0;
	/* Stop everything else related to ADCs: */
	ADCON1 = 0x0;
	ADCON2 = 0x0;
	disable_interrupt();
	ADIC = 0x0;
	enable_interrupt();
}

unsigned int adc_read(char channel) {
	char * adc_register;
	if(channel >= 8 || channel < 0) return 0; /* Invalid register index */

	/* Grab value from AD0, or AD1,..., AD7, by using a pointer: */	
	adc_register = (char*)((&AD0_ADDR) + channel);
	return *adc_register;
}

#pragma interrupt ADC_Interrupt
void ADC_Interrupt(void) {
	if(adc_cback) adc_cback();
}