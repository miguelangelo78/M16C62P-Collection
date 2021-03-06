/***********************************************************************/
/*                                                                     */
/*  FILE        :LEDs.cpp                                              */
/*  DATE        :Fri, Oct 16, 2015                                     */
/*  DESCRIPTION :main program file.                                    */
/*  CPU GROUP   :62P                                                   */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.19).    */
/***********************************************************************/
#include "sfr62p.h"

#define LED_INSIDE_DIR pd4
#define LED_OUTSIDE_DIR pd5
#define LED_INSIDE p4
#define LED_OUTSIDE p5

#define OUTPUT_PORT 0xFF

#define DELAY_DOWN 100
#define DELAY_UP 5000
#define DELAY_X 100

int del = DELAY_DOWN;
int del_down = 0;

void delay() {
	int i=0;
	for(;i<del;i++);
	
	if(del_down) {
		del += DELAY_X;
		if(del > DELAY_UP)
			del_down = 0;
	} else {
		del -= DELAY_X;
		if(del < DELAY_DOWN)
			del_down = 1;
	}
}

void leds_onboard() {
	LED_INSIDE = 0;
	delay();
	LED_INSIDE = 0xFF;
}

void leds_outside() {
	LED_OUTSIDE = 0xFF;
	delay();
	LED_OUTSIDE = 0;
}

void main(void) {
	LED_INSIDE_DIR = LED_OUTSIDE_DIR = OUTPUT_PORT;
	LED_INSIDE = 0xFF;
	
	for(;;) {
		leds_onboard();
		leds_outside();
	}	
}
