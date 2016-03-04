#include "../../globals.h"

/* DS1621 Datasheet: http://datasheets.maximintegrated.com/en/ds/DS1621.pdf */

#define TOUT P4_7
#define DIGITALTHERM_I2C_ADDRESS 0x49

/* Commands can be found in Page 10 of Datasheet */
enum DIGITAL_THERM_COMMANDS {
	CMD_READ_TEMP = 0xAA,
	CMD_TH = 0xA1,
	CMD_TL = 0xA2,
	CMD_CFG = 0xAC,
	CMD_COUNTER = 0xA8,
	CMD_SLOPE = 0xA9,
	CMD_START = 0xEE,
	CMD_STOP = 0x22
};

enum DIGITAL_THERMISTOR_MODE {
	MODE_CONT, MODE_ONESHOT	
};

/* Three ways to write to the Digital Thermistor: */
/* 1: */
void i2c_thermistor_send_cmd(unsigned char cmd) {
	i2c_send(DIGITALTHERM_I2C_ADDRESS, cmd);
}

/* 2: */
void i2c_thermistor_send_cmd_byte(unsigned char cmd, unsigned char byte) {
	unsigned char data[2];
	data[0] = cmd;
	data[1] = byte;
	i2c_send_str(DIGITALTHERM_I2C_ADDRESS, data, 2);
}

/* 3: */
void i2c_thermistor_send_cmd_word(unsigned char cmd, unsigned int word) {
	unsigned char data[3];
	data[0] = cmd;
	data[1] = (word & 0xFF00) >> 8; /* MSB */
	data[2] = word & 0x00FF; /* LSB */
	i2c_send_str(DIGITALTHERM_I2C_ADDRESS, data, 3); 
}

/* Two ways of reading from the Digital Thermistor: */
/* 1: */
char i2c_thermistor_read(unsigned char address) {
	return i2c_send_and_read(DIGITALTHERM_I2C_ADDRESS, address);
}

/* 2: */
int i2c_thermistor_read_word(unsigned char address) {
	char * ptr = i2c_send_and_read_str(DIGITALTHERM_I2C_ADDRESS, address, 2);
	return (int)(ptr[0] | (ptr[1] << 8)); /* Construct a word from 2 bytes, and return it */
}

void i2c_thermistor_start(unsigned char conversion_mode) {
	char data[2];
	data[0] = CMD_CFG; /* Config command */
	data[1] = 0x2 | (conversion_mode & 0x1); /* One shot or continuous mode */
	i2c_send_str(DIGITALTHERM_I2C_ADDRESS, data, 2);
	i2c_thermistor_send_cmd(CMD_START);
}

void i2c_thermistor_stop(void) {
	i2c_thermistor_send_cmd(CMD_STOP);
}

void i2c_thermistor_init(void) {
	P4D_7 = 0; /* TOUT pin on the M16C is an input. 
				  It indicates whether the temperature has reached a user defined temperature, like an alarm */
	i2c_init();
	i2c_thermistor_start(MODE_CONT); /* Start thermistor in continuous mode */
}

void i2c_thermistor_deinit(void) {
	i2c_thermistor_stop();
}

char i2c_thermistor_is_alarm(void) {
	return TOUT;
}

void i2c_thermistor_set_usertemp(thermistor_t mintemp, thermistor_t maxtemp) {
	i2c_thermistor_send_cmd_word(CMD_TL, mintemp);
	i2c_thermistor_send_cmd_word(CMD_TH, maxtemp);
}

thermistor_t i2c_thermistor_read_temp(void) {
	return i2c_thermistor_read_word(CMD_READ_TEMP);
}