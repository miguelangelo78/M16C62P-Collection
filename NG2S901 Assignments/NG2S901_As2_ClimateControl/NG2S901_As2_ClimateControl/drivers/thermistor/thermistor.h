#ifndef THERMISTOR_H_
#define THERMISTOR_H_

typedef int thermistor_t;

#define THERMISTOR_MAX_TEMP 40
#define THERMISTOR_MIN_TEMP 0

void thermistor_init(void);
void thermistor_deinit(void);
thermistor_t thermistor_read(void);

#endif