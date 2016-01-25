#include "sfr62p.h" 

/* 7 Segment display pins: */
#define SEGDISP_RIGHT_PWRDIR P4D_2
#define SEGDISP_RIGHT_PWR P4_2
#define SEGDISP_LEFT_PWRDIR P4D_3
#define SEGDISP_LEFT_PWR P4_3

/* 7 Segment power macros: */
#define SEG_RIGHT_TURNON() SEGDISP_RIGHT_PWRDIR = 1; SEGDISP_RIGHT_PWR = 1;
#define SEG_RIGHT_TURNOFF() SEGDISP_RIGHT_PWR = 0;
#define SEG_LEFT_TURNON() SEGDISP_LEFT_PWRDIR = 1; SEGDISP_LEFT_PWR = 1;
#define SEG_LEFT_TURNOFF() SEGDISP_LEFT_PWR = 0;

/* Actual macros for displaying data into the displays */
#define SEG_LEFT(dat) SEG_RIGHT_TURNOFF(); SEG_LEFT_TURNON(); P5 = dat; SEG_DELAY();	
#define SEG_RIGHT(dat) SEG_LEFT_TURNOFF(); SEG_RIGHT_TURNON(); P5 = dat; SEG_DELAY();	

/* Allows data to be shown on both 7 segment displays */
#define SEG_DELAY() for(i=0;i<300;i++);

void init_7seg() {
	/* 7 Segment input pins: */
	P5D = 0xFF;
	P5 = 0;
	
	/* Power on both 7 segment leds: */
	SEG_RIGHT_TURNON();
	SEG_LEFT_TURNON();
}

int bcd_table[10] = {
	1 | 2 | 4 | 8 | 16 | 32, /* 0 */
	2 | 4, /* 1 */
	1 | 2 | 8 | 16 | 64, /* 2 */
	1 | 2 | 4 | 8 | 64, /* 3 */
	2 | 4 | 32 | 64, /* 4 */
	1 | 4 | 8 | 32 | 64, /* 5 */
	1 | 4 | 8 | 16 | 32 | 64, /* 6 */
	1 | 2 | 4, /* 7 */
	1 | 2 | 4 | 8 | 16 | 32 | 64, /* 8 */
	1 | 2 | 4 | 8 | 32 | 64 /* 9 */
};

/* Compiler says 'too many initializer values' -.-' really? */
int bcd_table_hex[7] = {
	1 | 2 | 4 | 16 | 32 | 64, /* A */
	4 | 8 | 16 | 32 | 64, /* B */
	1 | 8 | 16 | 32, /* C */
	2 | 4 | 8 | 16 | 64, /* D */
	1 | 8 | 16 | 32 | 64, /* E */
	1 | 16 | 32 | 64, /* F */
	1 | 2 | 4 | 8 | 16 | 32 | 128 /* 0. (aka invalid number) */
};

unsigned char bin_2_bcd(unsigned char n) {
	if(n > 15) 
		return bcd_table_hex[6];
	if(n > 9) 
		return bcd_table_hex[n - 10];
	else 
		return bcd_table[n];
}

void seg_update(unsigned char n) {
	int i;
	/* Upper byte */
	SEG_LEFT(bin_2_bcd((n & 0xF0) >> 4));
	/* Lower byte */
	SEG_RIGHT(bin_2_bcd(n & 0xF));
}
