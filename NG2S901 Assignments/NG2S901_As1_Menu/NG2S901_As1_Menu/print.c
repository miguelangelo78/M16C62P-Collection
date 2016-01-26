#include "globals.h"

int printf(const char _far fmt[], ...) {
	char printf_buff[128];
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = vsprintf(printf_buff, fmt, args);
	va_end(args);
	puts(printf_buff);
	return ret;
}

/* Prints from left to right on a certain location */
int printf_at(char * fmt, unsigned char x, unsigned char y, ...) {
	Point p;
	char printf_at_buff[128];
	int ret;
	va_list args;
	va_start(args, fmt);
	va_arg(args, unsigned char); va_arg(args, unsigned char); /* Iterate to skip the 2nd and 3rd arguments */
	ret = vsprintf(printf_at_buff, fmt, args);
	va_end(args);
	p = GLCD_GoTo(x, y);
	puts(printf_at_buff);
	GLCD_GoTo(p.x, p.y);
	return ret;
}

/* Prints from right to left on a certain location. 
   Useful for printing on the right corner of the screen */
int printf_at_rl(char * fmt, unsigned char x, unsigned char y, ...) {
	Point p;
	char printf_at_buff[128];
	int ret;
	va_list args;
	va_start(args, fmt);
	va_arg(args, unsigned char); va_arg(args, unsigned char); /* Iterate to skip the 2nd and 3rd arguments */
	ret = vsprintf(printf_at_buff, fmt, args);
	va_end(args);
	p = GLCD_GoTo(x - strlen(printf_at_buff) * GLCD_CHAR_WIDTH + GLCD_CHAR_WIDTH, y);
	puts(printf_at_buff);
	GLCD_GoTo(p.x, p.y);
	return ret;
}