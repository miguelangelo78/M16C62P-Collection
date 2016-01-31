#include "globals.h"

char running;

void extra_close_cback(void) {
	running = 0;	
}

int extra(void) {
	char key, data;
	uart_init();
	
	install_cback(extra_close_cback, 0);
	running = 1;
	
	while(running) {
		/* Send whatever is typed on the keypad through UART: */
		if((key=getcommand())) uart_write(key);
		/* Write to the GLCD whatever UART receives asynchronously */
		if((data = uart_read_async())) putc(data);
	}
	
	return 0;
}