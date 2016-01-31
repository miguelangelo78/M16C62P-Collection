#include "globals.h"
#include <string.h>

#define PROGRAM_NAME(name) printf("Prog: %s\n_____________________\n", name);

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
				sev_seg_max_delay += 10;
				if(sev_seg_max_delay > 200) sev_seg_max_delay = 200;	
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
	/* Install callbacks for Int0, Int1 and ADC: */
	install_cback(int0_cback, 0);
	install_cback(int1_cback, 1);
	install_cback(adc_cback, 2);
	
	printf("-Int.0: Dim LEDs\n-Int.1: Count LEDs up\n\n-Use the pot. for the speed/brightness\n> Press %c to return", BACK);
	
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
				motor_tone(250,10);
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
	char passcode[] = "ABCD";
	int passlength = strlen(passcode);
	char * passinput = malloc(sizeof(char) * (passlength + 1));
	char passinput_ctr = 0;
	char pass_success = 0;
	
	memset(passinput, 0, passlength + 1); /* Clear out password input to 0s */
	prog4_running = 1;
	install_cback(int0_stop_prog4, 0);
	keypad_4x4_wait();
	
	PROGRAM_NAME("Pass secret");
	
	/* Ask pin code: */
	printf("Type pass code to access special program\n\n");
	GLCD_WriteString(" Pass code: ", Font_System7x8);
	while(prog4_running) {
		switch((key = getcommand())) {
			default:
				printf_at("%c                        ", 90 + passinput_ctr * GLCD_CHAR_WIDTH, 5, key);
				passinput[passinput_ctr++] = key;
				if(passinput_ctr >= passlength) {
					passinput[passinput_ctr] = '\0';
					/* Compare passwords */
					if(!strcmp(passinput, passcode)) {
						/* Correct password */
						printf_at("       Correct!", 90, 5);
						motor_tone(150, 45);
						motor_tone(550, 45);
						motor_tone(100, 45);
						motor_tone(90, 45);
						motor_tone(80, 45);
						printf_at("      Press a key to start", 90, 5);
						read_key();
						GLCD_ClearScreen();
						GLCD_GoTo(0, 0);
						PROGRAM_NAME("Pass secret");
						pass_success = 1;
						prog4_running = 0;
						break;
					} 
					/* Wrong password */
					motor_tone(500, 100);
					printf_at("       Wrong passkey!", 90, 5);
					memset(passinput, 0, passlength + 1);
					passinput_ctr = 0;
				} else {
					motor_tone(250,10);
				}
				
				keypad_4x4_wait();
				break;
			case 0: break; /* No key pressed */	
		}		
	}
	
	if(!pass_success) return 0; /* This means we broke out of the loop using the interrupt */
	/* Pin code successful: */
	
	prog4_running = 1;
	
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
	return 0;
}

/****************** PROGRAM 5: *******************/
struct {
	int mil, sec, min, hour;
} stopwatch; 

char counting = 0;

void prog5_timer_callback(void) {
	if(!counting) return;
	stopwatch.mil++;
	if(stopwatch.mil >= 60) { stopwatch.mil = 0; stopwatch.sec++; }
	if(stopwatch.sec >= 60) { stopwatch.sec = 0; stopwatch.min++; }
	if(stopwatch.min >= 60) { stopwatch.min = 0; stopwatch.hour++; }
	if(stopwatch.hour >= 24) { stopwatch.hour = 0; stopwatch.min = 0; stopwatch.sec = 0; stopwatch.mil = 0; }
} 

int timer_set(void) {
	char running = 1;
	char key;
	char old_sec = 0;
	int cooldown = 0;
	int keypad_cooldown = 0;
	char setting_time = 0;
	int set_cursor = 0;
	
	PROGRAM_NAME("Stopwatch");
	counting = 0;
	keypad_4x4_wait();
	
	printf("%c- Return   1- Reset", BACK);
	printf_at("2- Start/stop", 0, GLCD_HEIGHT-1);
	printf_at_rl("A- Set", GLCD_WIDTH-1, GLCD_HEIGHT-1);	
	
	timer_init(prog5_timer_callback, TIMERA1, 1, TIMER_MODE_NORMAL, 0, TIMER_CLK_F32, TIMER_PRI_1);

	while(running) {
		printf_at("%.2d:%.2d%c%.2d:%.2d", 35, 4, stopwatch.hour, stopwatch.min, (old_sec==stopwatch.sec ? ':' : ' ') , stopwatch.sec, stopwatch.mil);
		
		if(setting_time)
			printf_at("__", 35+(set_cursor*(GLCD_CHAR_WIDTH+3))*2,3);
		
		if(old_sec != stopwatch.sec && cooldown++ > 100) {
			old_sec = stopwatch.sec;
			cooldown = 0;
		}
		
		switch((key = getcommand())) {
		case '1': 
			stopwatch.mil = stopwatch.sec = stopwatch.min = stopwatch.hour = 0; 
			break;
		case '2': 
			if(!setting_time) { 
				counting = !counting; 
				keypad_4x4_wait(); 
			}
			break;
		case 'A': 
			setting_time = !setting_time; 
			if(setting_time) {
 				counting = 0;
				set_cursor = 0;
				printf_at("*/# to left/right\nC/D to change",0,5);
			} else {
				counting = 1;
				printf_at("                 ",0,5);
				printf_at("                 ",0,6);
				printf_at("                     ", 35,3);
			}
			keypad_4x4_wait();
			break;
		case '#':
			if(set_cursor++ >= 3) set_cursor = 3; 
			printf_at("                     ", 35,3);
			keypad_4x4_wait();
			break;
		case '*':
			if(set_cursor-- <= 0) set_cursor = 0;
			printf_at("                     ", 35,3);
			keypad_4x4_wait();
			break;
		case 'C': 
			if(keypad_cooldown++<15) break;
			keypad_cooldown = 0;
			switch(set_cursor) {
				case 0: if(stopwatch.hour++>=24) stopwatch.hour = 24;  break;
				case 1: if(stopwatch.min++>=59) stopwatch.min = 59; break;
				case 2: if(stopwatch.sec++>=59) stopwatch.sec = 59; break;
				case 3: if(stopwatch.mil++>=59) stopwatch.mil = 59; break;
			}
			break;
		case 'D': 
			if(keypad_cooldown++<15) break;
			keypad_cooldown = 0;
			switch(set_cursor) {
				case 0: if(stopwatch.hour--<=0) stopwatch.hour = 0;  break;
				case 1: if(stopwatch.min--<=0) stopwatch.min = 0; break;
				case 2: if(stopwatch.sec--<=0) stopwatch.sec = 0; break;
				case 3: if(stopwatch.mil--<=0) stopwatch.mil = 0; break;
			}
			break;
		case BACK: running = 0; break;
		}
	}
	
	return 0;
}

/****************** PROGRAM 6: *******************/
int ctr1 = 0;
void timer_cascaded_callback1(void) {
	printf_at("Timer 2: %d", 0,3, ctr1++);
}

int ctr2 = 0;
void timer_cascaded_callback2(void) {
	printf_at("Timer 3: %d", 0, 2, ctr2++);
}

int timer_cascade_int(void) {
	char running = 1;
	PROGRAM_NAME("Timer cascading");
	
	timer_install_cback(timer_cascaded_callback1, TIMERA2);
	timer_install_cback(timer_cascaded_callback2, TIMERA3);
	TA2 = 99;
	TA2MR = 0x01;
	TRGSR = 0x0C;
	TA3MR = 0x80;
	TA3 = PERIOD_TO_PRELOAD(f1_CLK_SPEED, 32, 0.005);
	
	disable_interrupt();
	TA2IC = 0x03;	
	TA3IC = 0x04;	
	enable_interrupt();
	
	TABSR_2 = 1;
	TABSR_3 = 1;
	
	printf_at_rl("Press %c to return", GLCD_WIDTH-1, GLCD_HEIGHT-1, BACK);
	
	while(running) {
		switch(getcommand()) {
		case BACK: running = 0; break;	
		}	
	}
		
	return 0;	
}

/* Show program intro explaining navigation controls: */
void intro(void) {
	char i;
	GLCD_Rectangle(0, 0, GLCD_WIDTH+5, 5);
	GLCD_WriteString("\n\n    NG2S901\n\n  Assignment 1\n", Font_System7x8);
	printf_at("Press any key!", GLCD_WIDTH/2-40, GLCD_HEIGHT-1);
	GLCD_Rectangle(0, 48, GLCD_WIDTH+5, 48);
	read_key();
}

int info(void) {
	printf("--------------------\nMade by Miguel Santos\n\n14031329 MEng C.S.E.\n\nHanded on 01/02/16\n--------------------\n> Any key to return");
	read_key();
	return 0;
}
