#include "sfr62p.h"
#include "lcd_driver.h"
#include "cpufreq.h"
#include "delay.h"

void main(void) {
	cpu_init(); /* throttle the cpu up! */
	lcd_init();
	lcd_cls();
	
	lcd_circ(10,10,10);
	lcd_rect(20,20,20,20);
	
	for(;;);
}
