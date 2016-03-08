#include "../../globals.h"
#include <string.h>

/* I2C Datasheet: http://documentation.renesas.com/doc/products/mpumcu/apn/m16c/rej05b1349_m16cap.pdf */

#define ACK (0)
#define NACK (!ACK)

char i2c_busy = 0;
char i2c_is_inititialized = 0;

schedule_t sched = 0, sched_copy = 0;

void i2c_set_schedule(schedule_t func_ptr) {
	sched = func_ptr;
}

char i2c_is_scheduled(void) {
	return sched != 0 || sched_copy != 0;	
}

char i2c_is_busy(void) {
	return i2c_busy;	
}

void i2c_delay(void) {
	unsigned int delay;
	for(delay = 0; delay < 5000; delay++);
}

void i2c_init(void) {
	if(i2c_is_inititialized) return;
	
	/* SDA and SCL pins as open collector */
	P7_0 = P7_1 = 1;
	P7D_0 = P7D_1 = 0;
	
	U2BRG = 0xFF;
	U2MR = 0x02; /* I2C Mode */
	U2SMR = 0x03; /* I2C Mode */
	U2SMR2 = 0x00;
	U2SMR3 = 0x02;
	U2C0 = 0x82; /* Select Clock */
	U2C1 = 0x05; /* Enable TX and RX */
	
	i2c_is_inititialized = 1;
}

void i2c_deinit(void) {
	if(!i2c_is_inititialized) return;
	i2c_stop();
	i2c_is_inititialized = 0;
}

void i2c_start(void) {
	i2c_busy = 1;
	i2c_delay();
	STAREQ_U2SMR4 = 1;
	i2c_delay();
	STSPSEL_U2SMR4 = 1;
	i2c_delay();
	STSPSEL_U2SMR4 = 0;
}

void i2c_restart(void) {
	i2c_busy = 1;
	i2c_delay();
	RSTAREQ_U2SMR4 = 1,
	i2c_delay();
	STSPSEL_U2SMR4 = 1;
	i2c_delay();
	STSPSEL_U2SMR4 = 0;
}

void i2c_stop(void) {
	i2c_delay();
	STPREQ_U2SMR4 = 1;
	i2c_delay();
	STSPSEL_U2SMR4 = 1;
	i2c_delay();
	STSPSEL_U2SMR4 = 0;
	i2c_busy = 0;
	
	/* Execute scheduled tasks, due to i2c being busy at the some previous time: */
	if(sched) {
		sched_copy = sched;
		sched = 0; /* Clear schedule first */
		sched_copy(); /* Run the copy instead */
		sched_copy = 0; /* Clear copy as well */
	}
}

void i2c_flush_tx(void) {
	TE_U2C1 = 1;
	while(!TI_U2C1);
	while(!TXEPT_U2C0);	
}

void i2c_enable_rx(void) {
	i2c_flush_tx(); /* Finish transmitting first */
	RE_U2C1 = 1; /* Enable RX */
}

void i2c_disable_rx(void) {
	RE_U2C1 = 0;
}

void i2c_enable_tx(void) {
	TE_U2C1 = 1;
	while(!TI_U2C1);
	IR_S2RIC = 0;
	IR_S2TIC = 0;
}

void i2c_disable_tx(void) {
	TE_U2C1 = 0;
}

void i2c_wait_n_ack(void) {
	while((!IR_S2RIC) && (!IR_S2TIC));	
}

/* If this returns a 1, then it's a NACK, otherwise, it's an ACK */
char i2c_is_nack(void) {
	return (U2RBH & 0x01 == 0x01);
}

/* If 1, then we're NACKing, else we're ACKing */
void i2c_set_ack(char nack) {
	U2TBH = nack ? NACK : ACK;
}

/* Send general purpose byte */
char i2c_transmit(unsigned char byte) {
	i2c_enable_tx();
	U2TBL = byte;
	i2c_wait_n_ack();
	i2c_disable_tx();
	return i2c_is_nack();
}

/* General purpose function to send a single byte to a certain address */
void i2c_send(unsigned char address, unsigned char data) {
	i2c_set_ack(NACK);
	i2c_start();
	i2c_transmit(address << 1);
	i2c_transmit(data);	
	i2c_stop();
}

/* General purpose function to send a multiple bytes to a certain address */
void i2c_send_str(unsigned char address, unsigned char * data, unsigned char datalen) {
	unsigned char i;
	i2c_set_ack(NACK);
	i2c_start();
	i2c_transmit(address << 1);
	for(i = 0; i < datalen; i++)
		i2c_transmit(data[i]);
	i2c_stop();
}

unsigned char i2c_read(unsigned char address) {
	unsigned char reg;
	i2c_set_ack(NACK);
	i2c_enable_rx();
	i2c_start();
	i2c_transmit(address << 1 | 1);
	i2c_transmit(0xFF);
	reg = U2RBL;
	i2c_set_ack(NACK); /* Master NACK (since we only read 1 byte, no ACKs were sent) */
	i2c_stop();
	return reg;
}

#define I2C_BUFFER_SIZE 32
unsigned char i2c_read_buffer[I2C_BUFFER_SIZE];

unsigned char * i2c_read_buff(unsigned char address, unsigned char bufflen) {
	unsigned char i;
	memset(i2c_read_buffer, 0, I2C_BUFFER_SIZE);
	i2c_enable_rx();
	i2c_start();
	i2c_transmit(address << 1 | 1);
	i2c_set_ack(ACK);
	for(i = 0;i < bufflen; i++) {
		i2c_transmit(0xFF);
		i2c_read_buffer[i] = U2RBL;
		if(i < bufflen-1)
			i2c_set_ack(ACK); /* ACK from Master */
		else  {
			i2c_set_ack(NACK); /* NACK from Master */
			i2c_transmit(0xFF);
			break;
		}
	}
	i2c_stop();
	i2c_read_buffer[bufflen] = -1; /* 255 as unsigned */
	return i2c_read_buffer;
}

unsigned char i2c_send_and_read(unsigned char address, unsigned char send_data) {
	unsigned char reg;
	/* Send byte first: */
	i2c_set_ack(NACK);
	i2c_start();
	i2c_transmit(address << 1); /* As a write */
	i2c_transmit(send_data);
	
	/* Then restart and read byte: */	
	i2c_enable_rx();
	i2c_restart();
	i2c_transmit(address << 1 | 1); /* As a read */
	i2c_transmit(0xFF);
	i2c_stop();
	reg = U2RBL;
	i2c_set_ack(NACK); /* Master NACK (since we only read 1 byte, no ACKs were sent) */
	return reg;
}

unsigned char * i2c_send_and_read_str(unsigned char address, unsigned char send_data, unsigned char bufflen) {
	unsigned char i;
	/* Send byte first: */
	i2c_set_ack(NACK);
	i2c_start();
	i2c_transmit(address << 1); /* As a write */
	i2c_transmit(send_data);
	
	/* Then restart and read byte: */	
	i2c_enable_rx();
	i2c_restart();
	i2c_transmit(address << 1 | 1); /* As a read */
	i2c_set_ack(ACK);
	for(i = 0;i < bufflen; i++) {
		i2c_transmit(0xFF);
		i2c_read_buffer[i] = U2RBL;
		if(i < bufflen-1)
			i2c_set_ack(ACK); /* ACK from Master */
		else  {
			i2c_set_ack(NACK); /* NACK from Master */
			i2c_transmit(0xFF);
			break;
		}
	}
	i2c_stop();
	i2c_read_buffer[bufflen] = -1; /* 255 as unsigned */
	return i2c_read_buffer;
}

unsigned char i2c_ping_slave(unsigned char address) {
	unsigned char success;
	i2c_set_ack(NACK);
	i2c_start();
	success = i2c_transmit(address << 1);
	i2c_stop();
	return success == 0; /* If success == 0, then the slave ACK'd back */
}