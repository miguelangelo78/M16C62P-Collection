#include "../../globals.h"

uart_rx_cback_t uart_cback_list[1];

#define BAUD_RATE 9600

void delay(unsigned long delay) {
	for(; delay; --delay);
}

void uart_init(void) {
	U0BRG = (unsigned char)(((f1_CLK_SPEED/16)/BAUD_RATE)-1);	

	/* UART Transmit/Receive Control Register 2 */  	  
	UCON = 0x00; 		
		 /*   00000000
		  b0	U0IRS		UART0 transmit irq cause select bit, 0 = transmit buffer empty 
	   	  b1	U1IRS		UART1 transmit irq cause select bit, 0 = transmit buffer empty
	   	  b2	U0RRM		UART0 continuous receive mode enable bit, set to 0 in UART mode
	  	  b3	U1RRM		UART1 continuous receive mode enable bit, set to 0 in UART mode
	   	  b4	CLKMD0		CLK/CLKS select bit 0, set to 0 in UART mode
	   	  b5	CLKMD1		CLK/CLKS select bit 1, set to 0 in UART mode
	   	  b6	RCSP		Separate CTS/RTS bit, 
	  	  b7	Reserved, set to 0 */

	/* UART0 transmit/receive control register 0 */
	/* f1 count source, CTS/RTS disabled, CMOS output */
	U0C0 = 0x10; 		
		 /* 00010000
		  b1:b0	CLK01:CLK0	BRG count source select bits
	   	  b2 	CRS 		CTS/RTS function select bit
	  	  b3	TXEPT		Transmit register empty flag
	   	  b4	CRD			CTS/RTS disable bit
	   	  b5	NCH			Data output select bit
	   	  b6	CKPOL		CLK polarity select bit,set to 0 in UART mode
	  	  b7	UFORM		Transfer format select bit,set to 0 in UART mode */

	/* UART0 transmit/receive control register 1 */
	/* 	disable transmit and receive, no error output pin, data not inverted */
  	U0C1 = 0x00; 		
		 /*  00000000 
		  b0	TE			Transmit enable bit
	   	  b1	TI			Transmit buffer empty flag
	   	  b2	RE			Receive enable bit
	  	  b3	RI			Receive complete flag
	   	  b5:b4				Reserved, set to 0
	   	  b6	UOLCH		Data logic select bit
	  	  b7	UOERE		Error signal output enable bit */

	/* UART0 transmit/receive mode register */
	/* 8-bit data,asynch mode, internal clock, 1 stop bit, no parity */
  	U0MR = 0x05;		
		 /*  00000101
		  b2:b0	SMD12:SMD1	Serial I/O Mode select bits
	   	  b3	CKDIR		Internal/External clock select bit, CKDIR
	   	  b4	STPS		Stop bit length select bit, STPS
	   	  b5	PRY			Odd/even parity select bit, PRY
	   	  b6	PRYE		Parity enable bit, PRYE
	  	  b7	IOPOL		TxD, RxD I/O polarity reverse bit */

	/* clear UART0 receive buffer by reading */
  	U0TB = U0RB;
	/* clear UART0 transmit buffer */
  	U0TB = 0;			

	/* disable irqs before setting irq registers */
    disable_interrupt();			
	/* Enable UART0 receive interrupt, priority level 4 */		
	//S0RIC = 0x04;
	/* Enable all interrupts */			
	enable_interrupt();			

	/* UART0 transmit/receive control register 1 */
	/* enable transmit and receive */
	U0C1 = 0x05; 		
		/*  00000101	enable transmit and receive  
		b0		TE			Transmit enable bit
		b1		TI			Transmit buffer empty flag
		b2		RE			Receive enable bit
		b3		RI			Receive complete flag
		b5:b4				Reserved, set to 0
		b6		UOLCH		Data logic select bit
		b7		UOERE		Error signal output enable bit */
}

void uart_deinit(void) {
	uart_uninstall_cback();
	U0BRG = 0;
	UCON = 0;
	U0C0 = 0;
	U0C1 = 0;
	U0MR = 0;
	U0TB = 0;
	disable_interrupt();
	S0RIC = 0;
	enable_interrupt();
}

void uart_write(char c) {
	while(TI_U0C1 == 0);
	U0TB = c;
}

void uart_writestr(char * str) {
	char i;
	for (i=0; str[i]; i++)
		uart_write(str[i]);
}

char uart_read(void) {
	int j;
	volatile char data;
	while(!(U0C1 & 0x08));
	data = U0RBL;
	for(j=0;j<2000;j++);
	return data;
}

char uart_read_async(void) {
	int j;
	volatile char data;
	if(!(U0C1 & 0x08)) return 0;
	data = U0RBL;
	for(j=0;j<2000;j++);
	return data;
}

void uart_install_cback(uart_rx_cback_t cback) {
	uart_cback_list[0] = cback;
}

void uart_uninstall_cback(void) {
	uart_cback_list[0] = 0;
}

void U0rec_ISR(void) {
	/* make sure receive is complete */
	while(RI_U0C1 == 0);
	
	/* read in received data */
	if(uart_cback_list[0]) uart_cback_list[0](U0RB);
}