#include "climate_control.h"
#include "globals.h"

void uart_receive(char byte) {
	printf_at("%c", 0,7,byte);
}

/* Communication happens on this function */
void bluetooth_control_worker(void) {
	char key;
	if((key=getcommand())) uart_write(key);
}

void bluetooth_control_init(void) {
	uart_init();
	uart_install_cback(uart_receive);
	/* Use a timer to control bluetooth as a background worker: */
	timer_init(bluetooth_control_worker, TIMERB0, 10, TIMER_MODE_NORMAL, 0, TIMER_CLK_F8, TIMER_PRI_3);
}
