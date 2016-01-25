#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdlib.h>
#include <stdarg.h>
#include <sfr62p.h>
#include "drivers/glcd/glcd.h" // include GLCD 
#include "drivers/glcd/font.h" // include fonts
#include "drivers/7seg/7segment.h"
#include "drivers/keypad/keyscan_4x4.h"
#include "drivers/motor/motor.h"
#include "programs.h"

/* We're using a default font from now on: */
#define puts(str) GLCD_WriteString(str, Font_System5x8)
#define putc(c) GLCD_WriteChar(c, Font_System5x8)
int printf_at_rl(char * fmt, unsigned char x, unsigned char y, ...);
int printf_at(char * fmt, unsigned char x, unsigned char y, ...);
int printf(const char _far fmt[], ...);

enum MENU_BUTTONS {
	OPEN = 'B',
	BACK = '5',
	UP = '3',
	DOWN = '9'	
};

inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif