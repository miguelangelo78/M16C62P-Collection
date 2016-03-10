#include "globals.h"
#include <string.h>

extern void set_temperature(thermistor_t new_desired_temperature);
extern void set_temperature_method(char method);
extern thermistor_t get_desired_temperature(void);
extern thermistor_t get_temperature(void);

extern char managing_climate;

enum BTOOTH_CMD {
	CMD_BYPASSCLIMATE,
	CMD_SETMOTOR,
	CMD_SETTHERMMETHOD,
	CMD_SETTEMP
};

#define CMD_FLAG 255

void btooth_exec_cmd(char cmd, unsigned char * data, int datalength) {
	switch(cmd) {
		case CMD_BYPASSCLIMATE: 
			managing_climate = data[0] & 0x1;
			break;
		case CMD_SETMOTOR:
			motor_spin(((unsigned int)(data[1])<<8) | (unsigned int)(data[0]), data[2]);
			break;
		case CMD_SETTHERMMETHOD: 
			set_temperature_method(data[0]);
			break;
		case CMD_SETTEMP: 
			set_temperature(data[0]);
			break;
		default: break;
	}
}

unsigned char uart_cmd_schedule_data[16];

void uart_cmd_schedule_cback(void) {
	/* NOW we can execute the command (after i2c has finished with its own business): */
	btooth_exec_cmd(uart_cmd_schedule_data[1], uart_cmd_schedule_data + 2, (int)(strchr(uart_cmd_schedule_data + 2, '\n') - (uart_cmd_schedule_data + 2)));
}

/* Trigger whenever we receive a string: */
void uart_receive_str(unsigned char * buff, int buffsize) {
	if(buff[0] == CMD_FLAG) /* We just received a command */
		if(!i2c_is_busy()) {
			/* 2nd byte contains the command, the 3rd contains the START of the data: */
			btooth_exec_cmd(buff[1], buff + 2, (int)(strchr(buff + 2, '\n') - (buff + 2)));
		} else {
			if(i2c_is_scheduled()) return; /* Uh oh, some function already scheduled I2C's transmission... */
			
			/* I2C is currently busy transmitting. Schedule this function to execute later */
			memcpy(uart_cmd_schedule_data, buff, buffsize);
			i2c_set_schedule(uart_cmd_schedule_cback);
		}
}

int i = 0;
unsigned char rx_buff[16];

/* Trigger whenever we receive a byte: */
void uart_receive(char byte) {
	rx_buff[i++] = byte;
	if(byte=='\n' || i==16) {
		uart_receive_str(rx_buff, i);
		memset(rx_buff, 0, i);
		i = 0;
	}
}

/* Communication happens on this function */
unsigned int btooth_preload = 0;

void bluetooth_control_worker(void) {
	unsigned char data_send_buff[7];
	unsigned int rpm;
	
	if(btooth_preload++>50) btooth_preload = 0;
	else return;
	
	data_send_buff[0] = 0;
	data_send_buff[1] = 0;
	data_send_buff[2] = get_temperature();
	data_send_buff[3] = get_desired_temperature();
	
	rpm = motor_read_rpm();
	data_send_buff[4] = rpm & 0xFF;
	data_send_buff[5] = (rpm & 0xFF00) >> 8;
	data_send_buff[6] = thermistor_type();
	uart_writebuff(data_send_buff, 7);
}

void bluetooth_control_init(void) {
	uart_init();
	uart_install_cback(uart_receive);
	/* Use a timer to control bluetooth as a background worker: */
	timer_init(bluetooth_control_worker, TIMERB0, 10, TIMER_MODE_NORMAL, 0, TIMER_CLK_F8, TIMER_PRI_3);
}

void bluetooth_show_rpm(void) {
	char str[32];
	char motor_direction;
	
	keypad_4x4_wait();
	GLCD_ClearScreen();
	while(!managing_climate) {
		GLCD_GoTo(0,0);
		sprintf(str, "\n\n      RPM");
		GLCD_WriteString(str, Font_System7x8);
		printf_at("        %d   \n\nDirection: %s  ", 
			0, 3, motor_read_rpm(), !(motor_direction=motor_get_direction())?"Left":(motor_direction==1?"Right":"Stopped"));
		
		if(getcommand()) managing_climate = 1;
	}
	GLCD_ClearScreen();
}