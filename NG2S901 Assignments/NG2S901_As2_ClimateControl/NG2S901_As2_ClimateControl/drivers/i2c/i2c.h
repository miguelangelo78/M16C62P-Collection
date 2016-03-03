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

#endif