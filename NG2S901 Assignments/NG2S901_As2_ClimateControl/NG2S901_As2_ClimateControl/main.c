#include "globals.h"

#define TEMP_ERROR 2
#define UPDATE_RATE 1000

char managing_climate;

thermistor_t current_temp;
thermistor_t desired_temp; /* TODO: Instead of setting desired_temp initially to current_temp, store it to EEPROM and load it from there */

void set_temperature(thermistor_t new_desired_temperature) {
	/* Change desired_temp */
	desired_temp = new_desired_temperature;
}

thermistor_t get_temperature(void) {
	return thermistor_read();	
}

unsigned int read_rpm(void) {
	return motor_read_rpm();
}

void climate_control(void) {
	unsigned int i;
	managing_climate = 1;
	while(managing_climate) {
		/* Read temperature and adjust motor in function of temperature: */
		current_temp = get_temperature();
		if(current_temp < desired_temp - TEMP_ERROR) /* Speed left, heat it up */
			motor_spin(map(current_temp, THERMISTOR_MAX_TEMP, THERMISTOR_MIN_TEMP, MOTOR_MIN_RPM, 9000), MOTOR_LEFT);
		else if(current_temp > desired_temp + TEMP_ERROR) /* Speed right, cool it down */
			motor_spin(map(current_temp, THERMISTOR_MIN_TEMP, THERMISTOR_MAX_TEMP, MOTOR_MIN_RPM, 9000), MOTOR_RIGHT);
		else /* Temperature match */
			motor_stop();
		
		/* Display data: */
		printf_at("Temp: %d'C \nDesired: %d'C\n%d RPM      ", 0,0, current_temp, desired_temp, read_rpm());
		for(i = 0; i < UPDATE_RATE; i++) 
			seg_update(current_temp, 10);
		
		printf_at("Time:\n%d seconds",0,4, rtc_get_seconds());	
	}
}

void main(void)
{
	Initialise();
	bluetooth_control_init();
	GLCD_Initialise();
	init_rtc();	
	keyscan_4x4_init();
	init_7seg();
	init_motor();
	thermistor_init();
	/* Set initial desired_temperature to the current temperature: */
	desired_temp = current_temp = thermistor_read();
	
	climate_control();
}