#include "../init/sfr62p.h" 
#include "motor.h"

void stop_motor(void) {
	P7_6 = P8_0 = 0;
}
