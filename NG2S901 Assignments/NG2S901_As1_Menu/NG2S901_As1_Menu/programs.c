#include "globals.h"

#define PROGRAM_NAME(name) printf("Prog: %s\n_____________________\n", name);

/******************* LED PATTERNS: ********************/
int shift = 0;
char shift_dir = 1;
unsigned int shift_delay = 0;
unsigned int shift_delay_max = 700;

#define DIM_DELAY_MAX 100
#define DIM_BRIGHTNESS_MAX (DIM_DELAY_MAX / 2)
unsigned int dim_delay = 0;
unsigned int dim_brightness = DIM_BRIGHTNESS_MAX/2;

unsigned int countup_delay = 0;
unsigned int countup_delay_max = 700;

enum LED_PATT_TYPE {
	LED_PATT_SHIFTSIDES,
	LED_PATT_DIM_ALL,
	LED_PATT_COUNTUP
};

void led_pattern_update(char patt_type) {
	switch(patt_type) {
	case LED_PATT_SHIFTSIDES: /* Shift leds sideways */
		if(shift_delay++ < shift_delay_max) return;
		shift_delay = 0;
		if(shift_dir) {
			P5 = (1 << shift++);
			if(shift >= 8)
				shift_dir = !shift_dir;
		} else {
			P5 = (1 << shift--);
			if(shift < 0)
				shift_dir = !shift_dir;
		}
		break;
	case LED_PATT_DIM_ALL: /* Dims all leds at the same time */
		if(dim_delay > DIM_DELAY_MAX/2 - dim_brightness && dim_delay < DIM_DELAY_MAX/2 + dim_brightness) 
			P5 = 0xFF;
		else 
			P5 = 0x00;	
		if(dim_delay++ > DIM_DELAY_MAX)
			dim_delay = 0;
		break;
	case LED_PATT_COUNTUP: /* Counts up the LEDs in binary */
		if(countup_delay++ > countup_delay_max) {
			countup_delay = 0;
			P5++;
		}
		break;
	}
	
	switch(getcommand()) {
	case '1': 
		if((shift_delay_max += 100) > 20000) shift_delay_max = 20000;
		break;
	case '2':
		if((shift_delay_max -= 100) < 500) shift_delay_max = 500;
		break;	
	}
}

/****************** PROGRAM 1: *******************/
int led_7seg(void) {
	char mode = 0;
	/* Store LEDs' register first: */
	char old_reg_dir = P5D;
	char old_reg = P5;
	char running = 1;
	char sev_seg_ctr = 0;
	char sev_seg_delay = 0;
	char sev_seg_max_delay = 150;

	init_7seg();
	P5D = 0xFF; /* LED port as output mode */
	
	/* Output program name: */
	PROGRAM_NAME("Led and 7Seg.");
	
	printf("\nPress 6 to toggle \nbetween LED and 7Seg.\n\nPress %c to return\nSpeed down - 1 up - 2", BACK);
	
	while(running) {
		/* Light up LEDs/7 Segment: */
		if(!mode) { /* Update LEDs: */
			led_pattern_update(LED_PATT_SHIFTSIDES);
		} else { /* Use 7 Segment display */
			seg_update(sev_seg_ctr); /* Update Seven segment display with the counter */
			if(sev_seg_delay++ > sev_seg_max_delay) {
				sev_seg_delay = 0;
				sev_seg_ctr++;
			}	
		}
		
		/* Read keypad: */
		switch(getcommand()) {
		case '1': /* Slow down seven segment display */
			if(mode) { /* Only react to this option when in 7segment mode */
				while(keypad_4x4_wait_async()) /* Wait for keypad to release and update the 7seg while waiting */
					seg_update(sev_seg_ctr); /* Update Seven segment display with the counter */
				sev_seg_delay = 0; 
				sev_seg_max_delay+=10;
				if(sev_seg_max_delay>200) sev_seg_max_delay = 200;	
			}
			break;
		case '2': /* Speed up seven segment display */ 
			if(mode) { /* Only react to this option when in 7segment mode */ 
				while(keypad_4x4_wait_async()) /* Wait for keypad to release and update the 7seg while waiting */
					seg_update(sev_seg_ctr); /* Update Seven segment display with the counter */
				sev_seg_delay = 0; 
				sev_seg_max_delay-=10;
				if(sev_seg_max_delay<10) sev_seg_max_delay = 10;
			}
		break;
		case '6': 
			if(!mode) {
				P5 = 0; /* Turn off LEDs */
				sev_seg_ctr = 0; /* Restart 7 Segment counter */
			}
			mode = !mode; /* Toggle mode */
			keypad_4x4_wait(); /* Wait for user to release button */
			break;
		case BACK: 
			running = 0; /* Stop program */
			break;
		}
	}
	
	/* Restore LEDs' registers: */
	P5 = old_reg;
	P5D = old_reg_dir;
	return 0;
}

/****************** PROGRAM 2: *******************/
char prog2_patt_type = LED_PATT_DIM_ALL;

char int0_cback(void) {
	prog2_patt_type = LED_PATT_DIM_ALL;
}

char int1_cback(void) {
	prog2_patt_type = LED_PATT_COUNTUP;
}

char adc_cback(void) {
	if(prog2_patt_type == LED_PATT_DIM_ALL) {
		/* Map the ADC value from 0 > 1023 to 0 > DIM_BRIGHTNESS_MAX:  */
		dim_brightness = (unsigned int) map(AD5, 0, 1023, 0, DIM_BRIGHTNESS_MAX);
	} else {
		/* Map the ADC value from 0 > 1023 to 0 > DIM_BRIGHTNESS_MAX:  */
		countup_delay_max = 1000 - (unsigned int) map(AD5, 0, 1023, 50, 1000);
	}
	ADST = 1;
}

int led_pattern(void) {
	char running = 1;
	char old_reg_dir = P5D;
	char old_reg = P5;
	PROGRAM_NAME("LEDs & Ints");
	install_cback(int0_cback, 0);
	install_cback(int1_cback, 1);
	install_cback(adc_cback, 2);
	
	printf("-Int.0: Dim LEDs\n-Int.2: Count LEDs up\n\n-Use the pot. for the speed/brightness\n> Press %c to return", BACK);
	
	while(running) {
		/* Update LED's animation: */
		led_pattern_update(prog2_patt_type);
		switch(getcommand()) {
			case BACK: running = 0; break;	
		}
	}
	
	/* Restore LEDs' registers: */
	P5 = old_reg;
	P5D = old_reg_dir;
	return 0;
}

/****************** PROGRAM 3: *******************/
char prog3_running = 1;

char int0_stop_prog3(void) {
	prog3_running = 0;
}

int keypad_scan(void) {
	char *key;
	prog3_running = 1;
	PROGRAM_NAME("Keypad & GLCD");
	install_cback(int0_stop_prog3, 0);
	printf("Press any key for testing. Int.0 to return");	
	/* Draw character borders: */

	printf_at("*********************", 0,  4);
	printf_at("*********************", 0,  6);
		
	while(prog3_running) {
		switch(getcommand()) {
			case 0: break; /* No key pressed, ignore */	
			default: 
				printf_at("Keys: %s", GLCD_WIDTH/2-28, 5, keypad_4x4_read_buffer()); 
				keypad_4x4_wait(); 
				printf_at("Keys:         ",GLCD_WIDTH/2-28, 5, keypad_4x4_read_buffer());
				break; /* Any other key pressed */
		}
	}
	return 0;
}

/****************** PROGRAM 4: *******************/
char prog4_running = 1;

char int0_stop_prog4(void) {
	prog4_running = 0;
}

int keypad_selection(void) {
	char running = 1;
	char base_tone = 1000;
	char base_tone_original = base_tone;
	char easter_egg = 0;
	char key;
	prog4_running = 1;
	install_cback(int0_stop_prog4, 0);
	init_motor_sound();
	keypad_4x4_wait();
	
	PROGRAM_NAME("Keypad Sel.");
	
	printf("- Use keys A,B,C and D to select different base tones.\n- Use the other keys for more sounds\n- Int.0 to return");
	
	while(prog4_running) {
		switch((key = getcommand())) {
		case 'A': base_tone = 1000; base_tone_original = base_tone; break;
		case 'B': base_tone = 200; base_tone_original = base_tone; break;
		case 'C': base_tone = 700; base_tone_original = base_tone; break;
		case 'D': base_tone = 300; base_tone_original = base_tone; break;
		case 0: break; /* No key pressed */
		default:
			/* Any other key */
			
			/* Ignore '#' and '*' */
			if(key == '*') { 
				easter_egg = 1;
				break;
			} else if (key == '#') {
				easter_egg = 0;
				break;	
			}
		
			key = key - '0' - 5;
			if(key > 0) {
				if(easter_egg)
					base_tone += key * 10;
				else
					base_tone = base_tone_original + key * 10;
			}
			else {
				if(easter_egg)
					base_tone -= key * 10;
				else
					base_tone = base_tone_original - key * 10;
			}
			break; 
		}
		motor_tone(base_tone, 1);
	}
	deinit_motor_sound();
	return 0;
}

/****************** PROGRAM 5: *******************/
int timer_set(void) {
	/* OPTION 3 */	
}

/****************** PROGRAM 6: *******************/
int timer_cascade_int(void) {
	/* OPTION 3 */
	
}

/* Show program intro explaining navigation controls: */
void intro(void) {
	GLCD_Rectangle(0, 0, GLCD_WIDTH+5, 5);
	GLCD_WriteString("\n\n    NG2S901\n\n  Assignment 1\n", Font_System7x8);
	printf_at("Press any key!", GLCD_WIDTH/2-40, GLCD_HEIGHT-1);
	GLCD_Rectangle(0, 48, GLCD_WIDTH+5, 48);
	read_key();
}

int info(void) {
	printf("----------\nMenu controls:\n3 - Up\n9 - Down\nB - Open menu/program\n5 - Back/return\n----------\nMade by M.S. 14031329");
	read_key();
	return 0;
}
