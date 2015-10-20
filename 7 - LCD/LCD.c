#include "lcd_driver.h"
#include "sfr62p.h"
#include "delay.h"

void main(void) {
	int ctr = '0';
	lcd_init();
	lcd_cls();
	
	lcd_circ(10,10,10);
	lcd_rect(20,20,20,20);
	for(;;) {
	}
}
