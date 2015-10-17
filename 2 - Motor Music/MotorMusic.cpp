#include "sfr62p.h"

#define MOTOR_LEFT(delay)  P7_6 = 1; P8_0 = 0; for(int i=0;i<delay;i++);
#define MOTOR_RIGHT(delay) P7_6 = 0; P8_0 = 1; for(int i=0;i<delay;i++);
#define MOTOR_VIBRATE(delay) MOTOR_LEFT(delay); MOTOR_RIGHT(delay);

#define MAX_MOTOR_DB 500
#define MIN_MOTOR_DB 0

void tone(int db, int duration_ms) {
	/* Check for invalid tones */
	if(db > MAX_MOTOR_DB || db < MIN_MOTOR_DB) return;

	for(; duration_ms; duration_ms--) {
		/* Restart timer */
		TA1IC = 0;
		TA1MR = 0x42;
		TA1 = 0x0800;
		TABSR_1 = 1;
		ONSF_1 = 1;
		
		for(;;) {
			/* Play the motor's sound */
			MOTOR_VIBRATE(db);
			
			/* Check when the timer has finished (should be 1ms) */
			if(TA1IC_3) break;
		}
		TABSR_1 = 1;
	}
	
	P7_6 = P8_0 = 0;	
}

void main(void) {
	P7D_6 = P8D_0 = 1;
	
	/* Laser sounds! */
	
	for(;;) {
		for(int i=0;i<MAX_MOTOR_DB;i+=5)
			tone(i, 1);
		for(int i=MAX_MOTOR_DB;i>=MIN_MOTOR_DB;i-=5)
			tone(i, 1);
	}
}
