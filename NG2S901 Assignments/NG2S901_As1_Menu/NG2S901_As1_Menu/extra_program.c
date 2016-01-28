#include "globals.h"

char running;

void uart_cback(char byte) {
	printf("%c", byte);
}

void extra_close_cback(void) {
	running = 0;	
}

int extra(void) {
	char key;
	uart_init();
	uart_install_cback(uart_cback);
	
	install_cback(extra_close_cback, 0);
	running = 1;
	
	while(running)
		if((key=getcommand())) uart_write(key);
	
	return 0;
}