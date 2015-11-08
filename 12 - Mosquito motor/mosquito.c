#include "sfr62p.h"
#include "cpufreq.h"

#define MOTOR_LEFT(delay)  p7_6 = 1; p8_0 = 0; for(i=0;i<delay;i++);
#define MOTOR_RIGHT(delay) p7_6 = 0; p8_0 = 1; for(i=0;i<delay;i++);
#define MOTOR_VIBRATE(delay) MOTOR_LEFT(delay); MOTOR_RIGHT(delay);
#define MOTOR_STOP() p7_6 = p8_0 = 0;

#define MAX_MOTOR_PITCH 500
#define MIN_MOTOR_PITCH 0

void tone(int pitch, int duration_ms) {
	int i;
	/* Check for invalid tones */
	if(pitch > MAX_MOTOR_PITCH || pitch < MIN_MOTOR_PITCH) return;

	for(; duration_ms; duration_ms--) {
		/* Restart timer */
		ta1ic = 0;
		ta1mr = 0x42;
		ta1 = 0x02;
		ta1s = 1;
		ta1os = 1;
		
		for(;;) {
			/* Play the motor's sound */
			MOTOR_VIBRATE(pitch);

			/* Check when the timer has finished (should be 1ms) */
			if(ir_ta1ic) break;
		}
		ta1s = 1;
	}

	MOTOR_STOP();
}

int brightness = 0;

#pragma interrupt adc(vect=14)
void adc(void) {
	brightness = (int)ad5/4;
	adst=1;
}

void main(void) {
	int i;
	
	cpu_init();
	pd5 = 0xFF;
	/* Initialize motor's direction pins as outputs: */
	pd7_6 = pd8_0 = 1;

	adcon0 = 0x05;
	adcon1 = 0x28;
	adcon2 = 0x05;
	_asm("fclr i");
	adic = 0x04;
	_asm("fset i");
	adst = 1;

	for(;;) {
		tone(brightness+100, 1);
	}
}
