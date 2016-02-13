#include "globals.h"

#define TEMP_ERROR 2

void main(void)
{
	thermistor_t temp;
	thermistor_t desired_temp = 20;
	Initialise();
	GLCD_Initialise();
	keyscan_4x4_init();
	init_motor();
	thermistor_init();
	
	for(;;) {
		temp = thermistor_read();
		if(temp < desired_temp - TEMP_ERROR)
			motor_spin(map(temp, 30, 10, MOTOR_MIN_RPM, 7000), MOTOR_LEFT);
		else if(temp > desired_temp + TEMP_ERROR)
			motor_spin(map(temp, 30, 10, MOTOR_MIN_RPM, 7000), MOTOR_RIGHT);
		else 
			motor_stop();
		printf_at("Temp: %d'C \nDesired: %d'C\n%d RPM      ", 0,0, temp, desired_temp, motor_read_rpm());
	}
}