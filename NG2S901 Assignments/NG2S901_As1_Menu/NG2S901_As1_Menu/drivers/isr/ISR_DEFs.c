#include "sfr62p.h"

/*******
 The timers are all defined in timer.c
 *******/

#define ISR_COUNT 3

typedef char (*isr_cback_t)(void);

isr_cback_t isr_callbacks[ISR_COUNT];

void uninstall_all_cback(void) {
	char i;
	for(i=0;i<ISR_COUNT;i++) isr_callbacks[i] = 0;
}

void init_isr(void) {
	uninstall_all_cback();
}

char priority_counter = 1;

char is_isr_regular_int(char cback_index) {
	return cback_index >=0 && cback_index < 2;
}

void set_isr_priority(char cback_index, char installing) {
	switch(cback_index) {
	case 0: INT0IC = installing ? priority_counter : 0; break;
	case 1: INT1IC = installing ? priority_counter : 0; break;
	case 2: /* Start up ADC: */
		ADCON0 = 0x05;
		ADCON1 = 0x28;
		ADCON2 = 0x05;
		_asm("fclr i");	
		ADIC = 0x04;
		_asm("fset i");
		ADST = 1;
		break;
	}
	
	/* Check if ISR needs its priority incremented */
	if(!is_isr_regular_int(cback_index)) return;
	
	if(installing && priority_counter++>7) 
		priority_counter = 7;
	else if(priority_counter--<1)
		priority_counter = 1; 
}

void install_cback(isr_cback_t callback, char cback_index) {
	isr_callbacks[cback_index] = callback;
	set_isr_priority(cback_index, 1);
}

void uninstall_cback(char cback_index) {
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