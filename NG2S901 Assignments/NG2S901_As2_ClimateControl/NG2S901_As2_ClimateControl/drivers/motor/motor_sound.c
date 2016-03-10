#include "../init/sfr62p.h" 
#include "motor.h"
#include <stdlib.h>

#define MOTOR_LEFT(delay)  P7_6 = 1; P8_0 = 0; for(i=0;i<delay;i++);
#define MOTOR_RIGHT(delay) P7_6 = 0; P8_0 = 1; for(i=0;i<delay;i++);
#define MOTOR_VIBRATE(delay) MOTOR_LEFT(delay); MOTOR_RIGHT(delay);
#define MOTOR_STOP() P7_6 = P8_0 = 0;

#define MIN_MOTOR_PITCH 0

void motor_tone_us(int pitch, int duration_us) {
	int i;
	/* Check for invalid tones */
	if(pitch > MOTOR_MAX_PITCH || pitch < MIN_MOTOR_PITCH) return;

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

void motor_tone(int pitch, int duration_ms) {
	int i;
	/* Check for invalid tones */
	if(pitch > MOTOR_MAX_PITCH || pitch < MIN_MOTOR_PITCH) return;
	
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

char oldreg_dir[2], oldreg[2];

void init_motor_sound(void) {
	/* Store registers first: */
	oldreg_dir[0] = P7D_6;
	oldreg_dir[1] = P8D_0;
	oldreg[0] = P7_6;
	oldreg[1] = P8_0;
	
	/* Init the registers: */
	P7D_6 = P8D_0 = 1;
}

void deinit_motor_sound(void) {
	/* Restore registers to original state: */
	P7D_6 = oldreg_dir[0];
	P8D_0 = oldreg_dir[1];
	P7_6 = oldreg[0];
	P8_0 = oldreg[1];
}
