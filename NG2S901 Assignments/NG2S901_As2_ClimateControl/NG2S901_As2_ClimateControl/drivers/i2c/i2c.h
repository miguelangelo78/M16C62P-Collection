#ifndef I2C_H_
#define I2C_H_

void i2c_init(void);
void i2c_deinit(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);
void i2c_send(unsigned char address, unsigned char data);
void i2c_send_str(unsigned char address, unsigned char * data, unsigned char datalen);
unsigned char i2c_read(unsigned char address);
unsigned char * i2c_read_buff(unsigned char address, unsigned char bufflen);
unsigned char i2c_send_and_read(unsigned char address, unsigned char send_data);
unsigned char * i2c_send_and_read_str(unsigned char address, unsigned char send_data, unsigned char bufflen);
unsigned char i2c_ping_slave(unsigned char address);
char i2c_is_busy(void);

typedef void (*schedule_t)(void);

void i2c_set_schedule(schedule_t);
char i2c_is_scheduled(void);

#endif