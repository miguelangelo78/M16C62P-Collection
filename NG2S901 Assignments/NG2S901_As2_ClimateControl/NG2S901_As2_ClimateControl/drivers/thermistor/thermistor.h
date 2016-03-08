#ifndef THERMISTOR_H_
#define THERMISTOR_H_

typedef int thermistor_t;

enum THERMISTOR_TYPE {
	THERMISTOR_ANALOGUE, THERMISTOR_DIGITAL
};

#define THERMISTOR_MAX_TEMP 40
#define THERMISTOR_MIN_TEMP 0

void thermistor_init(void);
void thermistor_deinit(void);
thermistor_t thermistor_read(void);
void thermistor_toggle(char thermistor_type);
char thermistor_type(void);

#endif