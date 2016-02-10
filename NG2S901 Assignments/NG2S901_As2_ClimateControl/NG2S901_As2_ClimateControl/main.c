#include "globals.h"

void main(void)
{
	Initialise();
	GLCD_Initialise();
	keyscan_4x4_init();
	init_motor_sound();
	thermistor_init();
	
	for(;;) {
		printf_at("%d  ", 0,0, thermistor_read());
	}
}