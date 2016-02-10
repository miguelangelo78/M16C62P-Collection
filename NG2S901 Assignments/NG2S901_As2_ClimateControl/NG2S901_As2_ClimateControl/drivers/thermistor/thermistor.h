#ifndef THERMISTOR_H_
#define THERMISTOR_H_

typedef char thermistor_t;

void thermistor_init(void);
void thermistor_deinit(void);
thermistor_t thermistor_read(void);

#endif