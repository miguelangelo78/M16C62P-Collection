#include "../../globals.h"

/*******
 The timers are all defined in timer.c
 *******/

#define ISR_COUNT 3

isr_cback_t isr_callbacks[ISR_COUNT];

char priority_counter = 1;

void uninstall_all_cback(void) {
	char i;
	for(i=0;i<ISR_COUNT;i++) isr_callbacks[i] = 0;
}

void init_isr(void) {
	uninstall_all_cback();
}

void adc_start(void) {
	ADST = 1;
}

void adc_stop(void) {
	ADST = 0;	
}

void init_adc(char channel) {
	bit_set(ADCON0, channel - ADC0_T);
}

void init_adc_basic(char channel) {
	bit_set(ADCON0, channel);
	ADCON1 = 0x20;
	ADCON2 = 0x05;
	disable_interrupt();
	ADIC = 0x04;
	enable_interrupt();
}

void deinit_adc_basic(char channel) {
	bit_clear(ADCON0, channel);
}

void deinit_all_adc(void) {
	int i;
	for(i=0;i<ADC_CHANNEL_COUNT;i++) deinit_adc_basic(i);
	ADCON1 = 0;
	ADCON2 = 0;
	disable_interrupt();
	ADIC = 0;
	enable_interrupt();
	adc_stop();
}

void set_isr_priority(char cback_index, char installing) {
	switch(cback_index) {
	case INT0_T: INT0IC = installing ? priority_counter : 0; break;
	case INT1_T: INT1IC = installing ? priority_counter : 0; break;
	}
	
	if(installing && priority_counter++>7) 
		priority_counter = 7;
	else if(priority_counter--<1)
		priority_counter = 1; 
}

void install_cback(isr_cback_t callback, char cback_index) {
	if(cback_index >= INTMAX || cback_index < 0) return; /* Invalid interrupt index */
	
	/* Install callback for just the ADCs: */
	if(cback_index >= ADC0_T && cback_index <= ADC2_T) {
		isr_callbacks[ADC0_T] = callback;
		init_adc_basic(cback_index - ADC0_T);
		return;
	}
	/* Install callbacks for INT0 and INT1 */
	isr_callbacks[cback_index] = callback;
	set_isr_priority(cback_index, 1);
}

void uninstall_cback(char cback_index) {
	if(cback_index >= INTMAX || cback_index < 0) return; /* Invalid interrupt index */

	/* Uninstall callback for just the ADCs: */
	if(cback_index >= ADC0_T && cback_index <= ADC2_T) {
		isr_callbacks[ADC0_T] = 0;
		deinit_adc_basic(cback_index - ADC0_T);
		return;
	}
	/* Uninstall callback for INT0 and INT1: */
	isr_callbacks[cback_index] = 0;
	set_isr_priority(cback_index, 0);
}

//ADC ISR

#pragma interrupt ADC_Interrupt
void ADC_Interrupt(void)
{
	if(isr_callbacks[2]) isr_callbacks[2]();
}
	
//INT0 ISR

#pragma interrupt INT0
void INT0(void)
{
	/* Jump to installed callback by the program */
	if(isr_callbacks[0]) isr_callbacks[0]();
}

//INT1 ISR

#pragma interrupt INT1
void INT1(void)
{
	/* Jump to installed callback by the program */
	if(isr_callbacks[1]) isr_callbacks[1]();
}