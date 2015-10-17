#include "sfr62p.h"
#include <stdlib.h>

#define MOTOR_LEFT(delay)  P7_6 = 1; P8_0 = 0; for(int i=0;i<delay;i++);
#define MOTOR_RIGHT(delay) P7_6 = 0; P8_0 = 1; for(int i=0;i<delay;i++);
#define MOTOR_VIBRATE(delay) MOTOR_LEFT(delay); MOTOR_RIGHT(delay);
#define MOTOR_STOP() P7_6 = P8_0 = 0;

#define MAX_MOTOR_PITCH 500
#define MIN_MOTOR_PITCH 0

void tone_us(int pitch, int duration_us) {
	/* Check for invalid tones */
	if(pitch > MAX_MOTOR_PITCH || pitch < MIN_MOTOR_PITCH) return;

	for(; duration_us; duration_us--) {
		/* Restart timer */
		TA1IC = 0;
		TA1MR = 0x42;
		TA1 = 0x02;
		TABSR_1 = 1;
		ONSF_1 = 1;
		
		for(;;) {
			/* Play the motor's sound */
			MOTOR_VIBRATE(pitch);
			
			/* Check when the timer has finished (should be 1ms) */
			if(TA1IC_3) break;
		}
		TABSR_1 = 1;
	}
	
	MOTOR_STOP();	
}

void tone(int pitch, int duration_ms) {
	/* Check for invalid tones */
	if(pitch > MAX_MOTOR_PITCH || pitch < MIN_MOTOR_PITCH) return;

	for(; duration_ms; duration_ms--) {
		/* Restart timer */
		TA1IC = 0;
		TA1MR = 0x42;
		TA1 = 0x0800;
		TABSR_1 = 1;
		ONSF_1 = 1;
		
		for(;;) {
			/* Play the motor's sound */
			MOTOR_VIBRATE(pitch);
			
			/* Check when the timer has finished (should be 1ms) */
			if(TA1IC_3) break;
		}
		TABSR_1 = 1;
	}
	
	MOTOR_STOP();	
}

void main(void) {
	/* Initialize motor's direction pins as outputs: */
	P7D_6 = P8D_0 = 1;
	
	/* Ultimate music generator! */
	for(;;)
		tone(rand()%500, rand()%100+1);
}
