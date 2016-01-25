#include <sfr62p.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef void (*timer_cback_t)(void);

#define enable_interrupts() _asm("fset i");
#define disable_interrupts() _asm("fclr i");

#define PERIOD_TO_PRELOAD(cpu_freq, freq_div, period) (period / (1 / cpu_freq))
#define HERTZ_TO_PRELOAD(cpu_freq, freq_div, hz) PERIOD_TO_PRELOAD(cpu_freq, freq_div, (1 / hz))

#define TIMER_COUNT 8

enum TIMERS {
	TIMERA0, TIMERA1, TIMERA2, TIMERA3, TIMERA4, TIMERB0, TIMERB1, TIMERB2
};

enum TIMER_MODE {
	TIMER_MODE_NORMAL, TIMER_MODE_EVENT, TIMER_MODE_ONESHOT, TIMER_MODE_PWM
};

enum TIMER_EDGE {
	TIMER_EDGE_LOW = 2, TIMER_EDGE_HIGH = 3	
};

enum TIMER_CLKSRC {
	TIMER_CLK_F1, TIMER_CLK_F8, TIMER_CLK_F32, TIMER_CLK_FC32	
}; 

enum TIMER_PRI {
	TIMER_PRI_0, TIMER_PRI_1, TIMER_PRI_2, TIMER_PRI_3, TIMER_PRI_4, TIMER_PRI_5, TIMER_PRI_6, TIMER_PRI_7, TIMER_PRI_IR	
};

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
	if(timer_cback_list[TIMERA0]) timer_cback_list[TIMERA2]();
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
	uint16_t timer_preload_period,
	uint8_t timer_mode, 
	uint8_t timer_trigger_edge, 
	uint8_t timer_clk_src, 
	uint8_t timer_priority
)  {
	int cpu_div = 1; /* TIMER_CLK_F1 by default */
	
	/* Set timer configuration: */
	uint8_t timer_config = (timer_clk_src << 6) | (timer_trigger_edge << 3) | timer_mode;
	if(timer_mode == TIMER_MODE_PWM) 
		timer_config |=  0b100;
	if(timer_clk_src == TIMER_CLK_F8)
		cpu_div = 8;
	else if(timer_clk_src == TIMER_CLK_F32 || timer_clk_src == TIMER_CLK_FC32)
		cpu_div = 32;
		
	/* Convert from period to preload: */
	// TODO: //timer_preload_period = (uint16_t)PERIOD_TO_PRELOAD(CPU_FREQ, cpu_div, timer_preload_period);
	
	/* Install cback into the array: */
	timer_cback_list[timer_index] = timer_callback;
	
	/* Initialize timer: */
	
	disable_interrupts();
	
	switch(timer_index) {
		case TIMERA0: /* Timer A0 has been reserved for the Keypad */
			/*TA0MR = timer_config;
			TA0IC = timer_priority;
			TA0 = timer_preload_period;
			TABSR_0 = 1;*/
			break;
		case TIMERA1:
			TA1MR = timer_config;
			TA1IC = timer_priority;
			TA1 = timer_preload_period;
			TABSR_1 = 1;
			break;
		case TIMERA2: 
			TA2MR = timer_config;
			TA2IC = timer_priority;
			TA2 = timer_preload_period;
			TABSR_2 = 1;
			break;
		case TIMERA3:
			TA3MR = timer_config;
			TA3IC = timer_priority;
			TA3 = timer_preload_period;
			TABSR_3 = 1;
			break;
		case TIMERA4: 
			TA4MR = timer_config;
			TA4IC = timer_priority;
			TA4 = timer_preload_period;
			TABSR_4 = 1;
			break;
		case TIMERB0:
			TB0MR = timer_config;
			TB0IC = timer_priority;
			TB0 = timer_preload_period;
			TABSR_5 = 1;
			break;
		case TIMERB1:
			TB1MR = timer_config;
			TB1IC = timer_priority;
			TB1 = timer_preload_period;
			TABSR_6 = 1;
			break;
		case TIMERB2: 
			TB2MR = timer_config;
			TB2IC = timer_priority;
			TB2 = timer_preload_period;
			TABSR_7 = 1;
			break;
		default: break; /* Invalid timer index */	
	}
	
	enable_interrupts();
}
