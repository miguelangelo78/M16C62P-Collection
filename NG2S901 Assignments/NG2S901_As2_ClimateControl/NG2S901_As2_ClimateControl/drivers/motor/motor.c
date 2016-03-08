#include "../../globals.h"

/* Macro to convert time (in ms) to RPM */
#define MS_TO_RPM(ms_elapsed) (60000 / (ms_elapsed))

unsigned int motor_rpm = 0; /* Current motor RPM value */
unsigned int motor_adjust_to_rpm = 0; /* Desired RPM value */
char spinning_direction = MOTOR_STOP;
unsigned int ms_elapsed = 0; /* Used to calculate rotation speed */
char encoder_oneshot = 0;

void motor_update_pwm(unsigned char duty_cycle) {
	if(spinning_direction == MOTOR_LEFT) { /* Spin left */
		TA3 = (duty_cycle * 65535) / 100;
		TA4 = 0;
	}
	else if(spinning_direction == MOTOR_RIGHT) { /* Spin right */
		TA4 = (duty_cycle * 65535) / 100;
		TA3 = 0;
	}
}

void motor_update_rpm(unsigned int new_desired_rpm) {
	if(new_desired_rpm > MOTOR_MAX_RPM) new_desired_rpm = MOTOR_MAX_RPM;
	if(new_desired_rpm < MOTOR_MIN_RPM) new_desired_rpm = MOTOR_MIN_RPM;
	
	/* Map the current RPM from 0 to 100, which is the duty cycle: */
	motor_update_pwm(map(new_desired_rpm, 0, MOTOR_MAX_RPM, 0, 100));
}

void encoder_trigger(void) {
	/* When this function runs it means the motor completed 1 rotation */
	/* Encoder has been triggered atleast once, let the motor block it again and trigger only when it unblocks once again: */
	if(encoder_oneshot) return;
	/* Update RPM value: */
	if(ms_elapsed > 0)
		motor_rpm = MS_TO_RPM(ms_elapsed);
	ms_elapsed = 0;
	encoder_oneshot = 1;
}

inline char encoder_read(void) {
	return P7_5;
}

void motor_cback(void) {
	/* Read encoder and update RPM's variable: */
	if(ms_elapsed++>30){  ms_elapsed = 0; motor_rpm = 0; } /* Took too much time to spin. It'll be considered 0 RPM */
	/* Call encoder_trigger only when the motor stops blocking the encoder */
	if(!encoder_read()) { encoder_trigger();  P4_0 = P4_1 = 0; }
	else { encoder_oneshot = 0; P4_0 = P4_1 = 1; } /* Let the encoder trigger again */
	
	/* If the desired RPM is 0 then just stop the motor: */
	if(!motor_adjust_to_rpm) { motor_update_pwm(0); return; }

	/* Adjust speed: */
	motor_update_rpm(motor_adjust_to_rpm);
	if(motor_rpm > motor_adjust_to_rpm) /* If the speed goes up too much, pull its speed down by 2300: */
		motor_update_rpm(motor_adjust_to_rpm - 2300);
}

void encoder_init(void) {
	unsigned int i = 0,j = 0;
	/* Encoder pin is P7_5 and is an input. When the encoder is blocked the pin P7_5 outputs 1, otherwise it's 0. */
	P7D_5 = 0;
	P4D_0 = P4D_1 = 1;
	/* Prevent the program from starting with the encoder unblocked: */
	while(1)
		if(!encoder_read()) { /* Read encoder */
			/* It's unblocked! Spin the motor a bit and check again if it's blocked */
			P7_6 = 1; P8_0 = 0;	/* Spin the motor a little */
			for(i=0;i<60000;i++);
			P7_6 = P8_0 = 0; /* Stop it */
			/* There is still a bit of momentum on the motor leftover, this momentum will ruin the RPM calculation. Wait a bit until it stops */
			for(i=0;i<60000;i++)
				for(j=0;j<4;j++);
		} else break; /* The sensor is blocked, it's safe to continue */
}

void init_motor(void) {
	init_motor_sound();
	encoder_init();
	/* Use timer to read the encoder, update the rpm and to adjust dynamically the rpm speed */
	timer_init(motor_cback, TIMERA2, 1, TIMER_MODE_NORMAL, 0, TIMER_CLK_F1, TIMER_PRI_1);

	/* Use 2 more timers, 1 for each direction: */
	/* Initialize the first with 0 speed: */
	timer_init(0, TIMERA3, 10, TIMER_MODE_PWM, 0, TIMER_CLK_F8, TIMER_PRI_2);
	TA3 = 0;
	/* Initialize the second with 0 speed: */
	timer_init(0, TIMERA4, 10, TIMER_MODE_PWM, 0, TIMER_CLK_F8, TIMER_PRI_2);
	TA4 = 0;
}

void deinit_motor(void) {
	P4_0 = P4_1 = 1;
	motor_stop();
	deinit_motor_sound();
	timer_deinit(TIMERA2);
	timer_deinit(TIMERA3);
	timer_deinit(TIMERA4);
}

void motor_stop(void) {
	spinning_direction = MOTOR_STOP;
	TA3 = TA4 = 0;
}

void motor_spin(unsigned int rpm, char direction) {
	/* Increase slowly the PWM value, keep reading the RPM from the sensor, and when the speed matches
	the desired one, oscillate around that value, due to motor's inertia */
	spinning_direction = direction & 0x3; /* Mask to collect only the first 2 bits */
	if(rpm > 0 && rpm < MOTOR_MIN_RPM) rpm = MOTOR_MIN_RPM;
	if(rpm >= MOTOR_MAX_RPM) rpm = 10000;
	motor_adjust_to_rpm = rpm;
}

unsigned int motor_read_rpm(void) {
	return motor_rpm;
}

char motor_get_direction(void) {
	return spinning_direction;	
}