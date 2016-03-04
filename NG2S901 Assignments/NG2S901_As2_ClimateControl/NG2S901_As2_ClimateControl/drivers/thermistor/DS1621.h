#ifndef DS1621_H_
#define DS1621_H_

typedef int thermistor_t;

void i2c_thermistor_init(void);
void i2c_thermistor_deinit(void);
thermistor_t i2c_thermistor_read_temp(void);
char i2c_thermistor_is_alarm(void);
void i2c_thermistor_set_usertemp(thermistor_t mintemp, thermistor_t maxtemp);

#endif