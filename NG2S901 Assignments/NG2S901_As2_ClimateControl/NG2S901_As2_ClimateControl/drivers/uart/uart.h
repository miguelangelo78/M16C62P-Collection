#ifndef UART_H_
#define UART_H_

/* UART receive callback: */
typedef void (*uart_rx_cback_t)(char byte);

void uart_init(void);
void uart_deinit(void);
void uart_write(char c);
void uart_writestr(char * str);
char uart_read(void);
char uart_read_async(void);
void uart_install_cback(uart_rx_cback_t cback);
void uart_uninstall_cback(void);

#endif