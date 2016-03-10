#ifndef GLOBALS_H_
#define GLOBALS_H_

/* This header is equivalent to the System API */

#include <stdlib.h>
#include <stdarg.h>
/* Include all drivers: */
#include "drivers/init/sfr62p.h"
#include "drivers/isr/ISR_DEFS.h"
#include "drivers/7seg/7segment.h"
#include "drivers/adc/adc.h"
#include "drivers/glcd/glcd.h"
#include "drivers/glcd/font.h"
#include "drivers/keypad/keyscan_4x4.h"
#include "drivers/motor/motor.h"
#include "drivers/timer/timer.h"
#include "drivers/thermistor/thermistor.h"
#include "drivers/uart/uart.h"
#include "drivers/i2c/i2c.h"
#include "drivers/rtc/DS1307.h"
#include "drivers/eeprom/M24C04.h"
#include "drivers/thermistor/DS1621.h"

/* We're using a default font from now on: */
#define puts(str) GLCD_WriteString(str, Font_System5x8)
#define putc(c) GLCD_WriteChar(c, Font_System5x8)
int printf_at_rl(char * fmt, unsigned char x, unsigned char y, ...);
int printf_at(char * fmt, unsigned char x, unsigned char y, ...);
int printf(const char _far fmt[], ...);

/* Map maps a variable from one range to another, for example, from 0 > 1024 to 0 > 255 */
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* Bit handling macros: */
#define bit_set(p,m) ((p) |= (1<<m))
#define bit_clear(p,m) ((p) &= ~(1<<m))
#define bit_write(c,p,m) (c ? bit_set(p, m) : bit_clear(p,m))
#define bit_get(p,m) ((p) & (1<<m))

#endif