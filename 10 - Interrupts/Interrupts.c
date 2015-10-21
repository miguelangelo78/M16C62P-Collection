#include "sfr62p.h"

#define enable_interrupt() _asm(" fset i");
#define disable_interrupt() _asm(" fclr i");

/* Interrupt declarations: */

/* Keypad interrupt: */
#pragma interrupt input_key(vect=13)
void input_key(void) {
	p5_0 = ~p5_0;
}

#pragma interrupt int0(vect=29)
void int0(void) {
	p5_1 = ~p5_1;
}

#pragma interrupt int1(vect=30)
void int1(void) {
	p5_2 = ~p5_2;
}

#pragma interrupt int2(vect=31)
void int2(void) {
	p5_3 = ~p5_3;
}

#pragma interrupt int3(vect=4)
void int3(void) {
	p5_4 = ~p5_4;	
}

void main(void) {
	/* Set keypad's interrupt: */
	pd10 = 0xF; /* half input, half output for keypad */
	kupic = 0x2; /* set priority */
	
	/* All the other interrupts: */
	int0ic = 1;
	int1ic = 2;
	int2ic = 3;
	int3ic = 4;
	 
	enable_interrupt();

	pd5 = 0xFF;	
	for(;;);
}
