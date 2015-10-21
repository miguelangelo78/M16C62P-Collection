#include "sfr62p.h"
#include "cpufreq.h"
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

#pragma interrupt timer_a0(vect=21)
void timer_a0(void) {
	if(timer_cback_list[TIMERA0]) timer_cback_list[TIMERA0]();
}

#pragma interrupt timer_a1(vect=22)
void timer_a1(void) {
	if(timer_cback_list[TIMERA1]) timer_cback_list[TIMERA1]();
}

#pragma interrupt timer_a2(vect=23)
void timer_a2(void) {
	if(timer_cback_list[TIMERA0]) timer_cback_list[TIMERA2]();
}

#pragma interrupt timer_a3(vect=24)
void timer_a3(void) {
	if(timer_cback_list[TIMERA3]) timer_cback_list[TIMERA3]();
}

#pragma interrupt timer_a4(vect=25)
void timer_a4(void) {
	if(timer_cback_list[TIMERA4]) timer_cback_list[TIMERA4]();
}

#pragma interrupt timer_b0(vect=26)
void timer_b0(void) {
	if(timer_cback_list[TIMERB0]) timer_cback_list[TIMERB0]();
}

#pragma interrupt timer_b1(vect=27)
void timer_b1(void) {
	if(timer_cback_list[TIMERB1]) timer_cback_list[TIMERB1]();
}

#pragma interrupt timer_b2(vect=28)
void timer_b2(void) {
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
		case TIMERA0:
			ta0mr = timer_config;
			ta0ic = timer_priority;
			ta0 = timer_preload_period;
			ta0s = 1;
			break;
		case TIMERA1:
			ta1mr = timer_config;
			ta1ic = timer_priority;
			ta1 = timer_preload_period;
			ta1s = 1;
			break;
		case TIMERA2: 
			ta2mr = timer_config;
			ta2ic = timer_priority;
			ta2 = timer_preload_period;
			ta2s = 1;
			break;
		case TIMERA3:
			ta3mr = timer_config;
			ta3ic = timer_priority;
			ta3 = timer_preload_period;
			ta3s = 1;
			break;
		case TIMERA4: 
			ta4mr = timer_config;
			ta4ic = timer_priority;
			ta4 = timer_preload_period;
			ta4s = 1;
			break;
		case TIMERB0: 
			tb0mr = timer_config;
			tb0ic = timer_priority;
			tb0 = timer_preload_period;
			tb0s = 1;
			break;
		case TIMERB1:
			tb1mr = timer_config;
			tb1ic = timer_priority;
			tb1 = timer_preload_period;
			tb1s = 1;
			break;
		case TIMERB2: 
			tb2mr = timer_config;
			tb2ic = timer_priority;
			tb2 = timer_preload_period;
			tb2s = 1;
			break;
		default: break; /* Invalid timer index */	
	}
	
	enable_interrupts();
}

void timer_foo(void) {
	p5_0 = ~p5_0;
}

void timer_foo2() {
	p5_1 = ~p5_1;	
}

void main(void) {
	cpu_init();
	timer_init(timer_foo, TIMERA3, 90000, TIMER_MODE_PWM, 0, TIMER_CLK_F8, TIMER_PRI_3);
	timer_init(timer_foo2, TIMERA0, 50000000, TIMER_MODE_NORMAL, 0, TIMER_CLK_F8, TIMER_PRI_4);
	
	pd5 = 0xFF;
	for(;;);
}
