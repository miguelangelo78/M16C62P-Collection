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

void set_isr_priority(char cback_index, char installing) {
	switch(cback_index) {
	case INT0_T: INT0IC = installing ? priority_counter : 0; break;
	case INT1_T: INT1IC = installing ? priority_counter : 0; break;
	case INT2_T: INT2IC = installing ? priority_counter : 0; break;
	}
	
	if(installing && priority_counter++ > 7) 
		priority_counter = 7;
	else if(priority_counter-- < 1)
		priority_counter = 1; 
}

void install_cback(isr_cback_t callback, char cback_index) {
	if(cback_index >= INTMAX || cback_index < 0) return; /* Invalid interrupt index */
	
	/* Install callbacks for INT0 and INT1 */
	isr_callbacks[cback_index] = callback;
	set_isr_priority(cback_index, 1);
}

void uninstall_cback(char cback_index) {
	if(cback_index >= INTMAX || cback_index < 0) return; /* Invalid interrupt index */

	/* Uninstall callback for INT0 and INT1: */
	isr_callbacks[cback_index] = 0;
	set_isr_priority(cback_index, 0);
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

//INT2 ISR

#pragma interrupt INT2
void INT2(void)
{
	/* Jump to installed callback by the program */
	if(isr_callbacks[2]) isr_callbacks[2]();
}
