#ifndef DELAY_H
#define DELAY_H

/* Don't forget to #include "sfr62p.h" on the .c files */

/* Note: These delay functions must be used AFTER setting the CPU's frequency to 20MHZ (?) */

#define RESTART_TIMER(preload) \
ta3ic = 0; \
ta3mr = 0x42; \
ta3 = preload; \
ta3s = 1; \
ta3os = 1; \

/* These variables will live on the stack, not statically on ram */
#define PUSH_TIMER() \
char ta3ic_reg = ta3ic; \
char ta3mr_reg = ta3mr; \
char ta3_reg = ta3; \
char ta3s_reg_bit = ta3s; \
char ta3os_reg_bit = ta3os;

#define POP_TIMER() \
ta3ic = ta3ic_reg; \
ta3mr = ta3mr_reg; \
ta3 = ta3_reg; \
ta3s = ta3s_reg_bit; \
ta3os = ta3os_reg_bit;

inline void delay_ms(int ms) {
	PUSH_TIMER();
	for(; ms; ms--) {
		RESTART_TIMER(0x900);
		while(!ir_ta3ic);
	}
	POP_TIMER();
}

inline void delay_us(int us) {
	PUSH_TIMER();
	for(; us; us--) {
		RESTART_TIMER(0x2);
		while(!ir_ta3ic);
	}
	POP_TIMER();
}

#endif