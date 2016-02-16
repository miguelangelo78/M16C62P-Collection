#ifndef CLIMATE_CONTROL_H_
#define CLIMATE_CONTROL_H_

#include "drivers/thermistor/thermistor.h"

/* This header provides access to external functions/modules to control the main program / climate control program */
extern void set_temperature(thermistor_t new_desired_temperature);
extern thermistor_t get_temperature(void);
extern unsigned int read_rpm(void);

#endif