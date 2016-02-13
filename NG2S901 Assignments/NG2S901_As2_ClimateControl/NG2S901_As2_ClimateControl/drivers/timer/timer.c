#include "../init/sfr62p.h"
#include "timer.h"
#include <stdlib.h>

timer_cback_t timer_cback_list[TIMER_COUNT];

/* Timer A0 has been reserved for the Keypad */
/*#pragma interrupt Timer_A0
void Timer_A0(void) {
	if(timer_cback_list[TIMERA0]) timer_cback_list[TIMERA0]();
}*/

#pragma interrupt Timer_A1
void Timer_A1(void) {	
	if(timer_cback_list[TIMERA1]) timer_cback_list[TIMERA1]();
}

#pragma interrupt Timer_A2
void Timer_A2(void) {
	if(timer_cback_list[TIMERA2]) timer_cback_list[TIMERA2]();
}

#pragma interrupt Timer_A3
void Timer_A3(void) {
	if(timer_cback_list[TIMERA3]) timer_cback_list[TIMERA3]();
}

#pragma interrupt Timer_A4
void Timer_A4(void) {
	if(timer_cback_list[TIMERA4]) timer_cback_list[TIMERA4]();
}

#pragma interrupt Timer_B0
void Timer_B0(void) {
	if(timer_cback_list[TIMERB0]) timer_cback_list[TIMERB0]();
}

#pragma interrupt Timer_B1
void Timer_B1(void) {
	if(timer_cback_list[TIMERB1]) timer_cback_list[TIMERB1]();
}

#pragma interrupt Timer_B2
void Timer_B2(void) {
	if(timer_cback_list[TIMERB2]) timer_cback_list[TIMERB2]();
}

void timer_init(
	timer_cback_t timer_callback, 
	uint8_t timer_index, 
	float timer_period,
	uint8_t timer_mode, 
	uint8_t timer_trigger_edge, 
	uint8_t timer_clk_src, 
	uint8_t timer_priority
)  {
	int cpu_div = 1; /* TIMER_CLK_F1 by default */
	
	/* Set timer configuration: */
	uint8_t timer_config = (timer_clk_src << 6) | (timer_trigger_edge << 3) | timer_mode;
	if(timer_mode == TIMER_MODE_PWM) /* Set PWM to 16 bit width by default */
		timer_config |=  0b100;
	if(timer_clk_src == TIMER_CLK_F8)
		cpu_div = 8;
	else if(timer_clk_src == TIMER_CLK_F32 || timer_clk_src == TIMER_CLK_FC32)
		cpu_div = 32;
	
	/* Convert from period to preload: */
	timer_period = PERIOD_TO_PRELOAD(f1_CLK_SPEED, cpu_div, (timer_period/1000 + 0.00318));
	
	/* Install cback into the array: */
	timer_cback_list[timer_index] = timer_callback;
	
	/* Initialize timer: */
	
	disable_interrupt();
	
	switch(timer_index) {
		case TIMERA0: /* Timer A0 has been reserved for the Keypad */
			/*TA0MR = timer_config;
			TA0IC = timer_priority;
			TA0 = timer_period;
			TABSR_0 = 1;*/
			break;
		case TIMERA1:
			TA1MR = timer_config;
			TA1IC = timer_priority;
			TA1 = timer_period;
			TABSR_1 = 1;
			break;
		case TIMERA2: 
			TA2MR = timer_config;
			TA2IC = timer_priority;
			TA2 = timer_period;
			TABSR_2 = 1;
			break;
		case TIMERA3:
			TA3MR = timer_config;
			TA3IC = timer_priority;
			TA3 = timer_period;
			TABSR_3 = 1;
			break;
		case TIMERA4: 
			TA4MR = timer_config;
			TA4IC = timer_priority;
			TA4 = timer_period;
			TABSR_4 = 1;
			break;
		case TIMERB0:
			TB0MR = timer_config;
			TB0IC = timer_priority;
			TB0 = timer_period;
			TABSR_5 = 1;
			break;
		case TIMERB1:
			TB1MR = timer_config;
			TB1IC = timer_priority;
			TB1 = timer_period;
			TABSR_6 = 1;
			break;
		case TIMERB2: 
			TB2MR = timer_config;
			TB2IC = timer_priority;
			TB2 = timer_period;
			TABSR_7 = 1;
			break;
		default: break; /* Invalid timer index */	
	}
	
	enable_interrupt();
}

void timer_deinit(uint8_t timer_index) {
	timer_cback_list[timer_index] = 0;
	disable_interrupt();
	
	switch(timer_index) {
		case TIMERA0: /* Timer A0 has been reserved for the Keypad */
			/*TA0MR = 0;
			TA0IC = 0;
			TA0 = 0;
			TABSR_0 = 0;*/
			break;
		case TIMERA1:
			TA1MR = 0;
			TA1IC = 0;
			TA1 = 0;
			TABSR_1 = 0;
			break;
		case TIMERA2: 
			TA2MR = 0;
			TA2IC = 0;
			TA2 = 0;
			TABSR_2 = 0;
			break;
		case TIMERA3:
			TA3MR = 0;
			TA3IC = 0;
			TA3 = 0;
			TABSR_3 = 0;
			break;
		case TIMERA4: 
			TA4MR = 0;
			TA4IC = 0;
			TA4 = 0;
			TABSR_4 = 0;
			break;
		case TIMERB0:
			TB0MR = 0;
			TB0IC = 0;
			TB0 = 0;
			TABSR_5 = 0;
			break;
		case TIMERB1:
			TB1MR = 0;
			TB1IC = 0;
			TB1 = 0;
			TABSR_6 = 0;
			break;
		case TIMERB2: 
			TB2MR = 0;
			TB2IC = 0;
			TB2 = 0;
			TABSR_7 = 0;
			break;
		default: break; /* Invalid timer index */	
	}
	
	enable_interrupt();
}

void timer_deinit_all(void) {
	char i;
	for(i=0;i<TIMER_COUNT;i++)
		timer_deinit(i);
}

void timer_install_cback(timer_cback_t timer_callback, uint8_t timer_index) {
	timer_cback_list[timer_index] = timer_callback;
}

void timer_uninstall_cback(uint8_t timer_index) {
	timer_cback_list[timer_index] = 0;	
}