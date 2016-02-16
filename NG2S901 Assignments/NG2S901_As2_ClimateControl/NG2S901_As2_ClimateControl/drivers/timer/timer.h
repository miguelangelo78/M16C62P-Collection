#ifndef TIMER_H_
#define TIMER_H_

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

#define PERIOD_TO_PRELOAD(cpu_freq, freq_div, period) ((unsigned short) (((cpu_freq / freq_div) * period) - 1))
#define HERTZ_TO_PRELOAD(cpu_freq, freq_div, hz) PERIOD_TO_PRELOAD(cpu_freq, freq_div, (1 / hz))

typedef void (*timer_cback_t)(void);

void timer_init(
	timer_cback_t timer_callback, 
	uint8_t timer_index, 
	float timer_period,
	uint8_t timer_mode, 
	uint8_t timer_trigger_edge, 
	uint8_t timer_clk_src, 
	uint8_t timer_priority
);

void timer_deinit(uint8_t timer_index);
void timer_deinit_all(void);
void timer_install_cback(timer_cback_t timer_callback, uint8_t timer_index);
void timer_uninstall_cback(uint8_t timer_index);

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

#endif