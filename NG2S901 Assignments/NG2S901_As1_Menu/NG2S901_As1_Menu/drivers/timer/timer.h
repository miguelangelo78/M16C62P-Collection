#ifndef TIMER_H_
#define TIMER_H_

void timer_init(
	timer_cback_t timer_callback, 
	uint8_t timer_index, 
	uint16_t timer_preload_period,
	uint8_t timer_mode, 
	uint8_t timer_trigger_edge, 
	uint8_t timer_clk_src, 
	uint8_t timer_priority
);

#endif