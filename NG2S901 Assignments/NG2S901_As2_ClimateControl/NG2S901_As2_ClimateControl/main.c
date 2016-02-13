#include "globals.h"

#define TEMP_ERROR 2
#define UPDATE_RATE 1000

void main(void)
{
	unsigned int i;
	thermistor_t temp;
	thermistor_t desired_temp = 20;
	
	Initialise();
	GLCD_Initialise();
	keyscan_4x4_init();
	init_7seg();
	init_motor();
	thermistor_init();
	
	for(;;) {
		/* Read temperature and adjust motor in function of temperature: */
		temp = thermistor_read();
		if(temp < desired_temp - TEMP_ERROR) /* Speed left, heat it up */
			motor_spin(map(temp, THERMISTOR_MAX_TEMP, THERMISTOR_MIN_TEMP, MOTOR_MIN_RPM, 7000), MOTOR_LEFT);
		else if(temp > desired_temp + TEMP_ERROR) /* Speed right, cool it down */
			motor_spin(map(temp, THERMISTOR_MIN_TEMP, THERMISTOR_MAX_TEMP, MOTOR_MIN_RPM, 7000), MOTOR_RIGHT);
		else /* Temperature match */
			motor_stop();
		
		/* Display data: */
		printf_at("Temp: %d'C \nDesired: %d'C\n%d RPM      ", 0,0, temp, desired_temp, motor_read_rpm());
		for(i = 0; i < UPDATE_RATE; i++)
			seg_update(temp, 10);
	}
}