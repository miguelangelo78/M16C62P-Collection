#include "globals.h"

#define TEMP_ERROR 2
#define UPDATE_RATE 500
#define PINCODE_LENGTH 5

void check_i2c(char i2c_crashed); /* Function prototype */

char managing_climate = 1;
char pincode[PINCODE_LENGTH]; /* The pincode must be loaded from EEPROM */

/* We need to compare these two temperatures in order to control the Motor: */
thermistor_t current_temp;
thermistor_t desired_temp;

/* Get real time temperature: */
thermistor_t get_temperature(void) {
	return current_temp;	
}

/* Get the temperature we want the thermistor to be at: */
thermistor_t get_desired_temperature(void) {
	return desired_temp;
}

/* Set the temperature of the thermistor to a new one: */
void set_temperature(thermistor_t new_desired_temperature) {
	/* Change desired_temp */
	desired_temp = new_desired_temperature;
	eeprom_write(0, desired_temp);
}

/* Is the thermistor Analogue or Digital: */
void set_temperature_method(char method) {
	thermistor_toggle(method & 0x1);	
}

/* Set pincode to a new one: */
void set_new_pincode(char * newpincode) {
	uint8_t i;
	for(i = 0;i < PINCODE_LENGTH;i++)
		pincode[i] = newpincode[i];
	eeprom_write_buff(2, PINCODE_LENGTH, newpincode);
}

/* Load pincode from eeprom: */
void load_pincode(void) {
	uint8_t i, * eeprom_ptr = eeprom_read_buff(2, PINCODE_LENGTH);
	for(i = 0;i < PINCODE_LENGTH; i++)
		pincode[i] = (char)eeprom_ptr[i];
}

/* We'll use this to read a pin from keypad and to return its address */
char pinBuffer[PINCODE_LENGTH];

char * read_pin(char * request_message) {
	char i;
	GLCD_ClearScreen();
	GLCD_GoTo(0,3);
	GLCD_WriteString(request_message, Font_System7x8);

	for(i = 0; i < PINCODE_LENGTH - 1;i++) {
		pinBuffer[i] = read_key();
		printf("*");
	}
	GLCD_ClearScreen();
	pinBuffer[PINCODE_LENGTH - 1] = '\0';
	return (char*)pinBuffer;
}

/* Read pin and compare with pincode: */
char authenticate(char * request_message) {
	return !strcmp(read_pin(request_message), (char*)pincode);
}

/* Output a valid/invalid message: */
void validate_authentication(char isValid) {
	if(isValid) {
		GLCD_GoTo(20,2);
		GLCD_WriteString("The pin is\n    Valid!\n\n", Font_System7x8);
		printf("Press any key to\ncontinue...");
		read_key();
		keypad_4x4_wait();
		keypad_4x4_flush();
	} else {
		GLCD_GoTo(10,2);
		GLCD_WriteString("The pin is NOT\n     Valid!\n\n", Font_System7x8);
		printf("Press any key to go\nback...");
		read_key();		
	}
}

/*************************** Setting's Menus: ****************************/
unsigned volatile int potVal = 0;

void pot_cback(void) {
	potVal = adc_read(5);
	ADC_RESTART();
}

void change_temperature_menu(void) {
	unsigned int new_temp, sliderPreload = 0;
	thermistor_deinit(); /* Uninitialize thermistor */
	/* And initialize potentiometer: */
	potVal = 0;
	adc_init(pot_cback, 5, ADC_MODE_ONESHOT, 1, 0, ADC_DEFAULT);
	
	printf("Settings > Set temp\n_____________________\n   New temperature:\n\n");
	printf_at("> (Use Pot.)\n> Any key to save",0,GLCD_HEIGHT-2);

	while(!getcommand()) {
		/* Reduce slider flickering using this reload value: */
		if(sliderPreload++ < 3000) continue;
		sliderPreload = 0;
			
		new_temp = map(potVal, 0, 1023, 0, 40); /* Map ADC from 0 > 1024 to 0 > 40ºC */
		printf_at("%d'C   \n\n---------------------", GLCD_WIDTH/2-5,3, new_temp);
		printf_at("|", map(new_temp, 0,40, 0, GLCD_WIDTH-1),5);
	}
	
	/* Save new temperature: */
	set_temperature(new_temp);
	
	adc_deinit(); /* Uninitialize potentiometer */
	thermistor_init(); /* Reinitialize thermistor */
}

void set_time_menu(void) {
	char key;
	char time_prepared = 0;
	char cursor_pos = 0;
	/* Grab current time: */
	rtc_t * new_rtc = read_clock();
	
	thermistor_deinit(); /* Uninitialize thermistor */
	/* And initialize potentiometer: */
	potVal = 0;
	adc_init(pot_cback, 5, ADC_MODE_ONESHOT, 1, 0, ADC_DEFAULT);
	
	printf("Settings > Set time\n_____________________\n       New time:\n\n");
	printf_at("> (Use Pot., 1 and 2)\n> Any key to save",0,GLCD_HEIGHT-2);
	
	while(!time_prepared) {
		switch(cursor_pos) {
		case 0: new_rtc->hour = map(potVal, 0, 1023, 0, 23); break;
		case 1: new_rtc->min = map(potVal, 0, 1023, 0, 59); break;
		case 2: new_rtc->sec = map(potVal, 0, 1023, 0, 59); break;	
		}
		
		printf_at("%.2d:%.2d:%.2d",GLCD_WIDTH/2-17 ,GLCD_HEIGHT-4, new_rtc->hour, new_rtc->min, new_rtc->sec);
		printf_at("--", 45+cursor_pos*(GLCD_CHAR_WIDTH*3), GLCD_HEIGHT-3);
		if((key = getcommand())) {
			keypad_4x4_flush();
			keypad_4x4_wait();
			switch(key) {
			case '1': printf_at("                     ",0,GLCD_HEIGHT-3); if(cursor_pos) cursor_pos--; break;
			case '2': printf_at("                     ",0,GLCD_HEIGHT-3); if(cursor_pos < 2) cursor_pos++; break;
			default: time_prepared = 1; /* The time is set, exit this function */ break;
			}
		}
	}
	
	/* Save new time: */
	rtc_set_clock(new_rtc);
	
	adc_deinit(); /* Uninitialize potentiometer */
	thermistor_init(); /* Reinitialize thermistor */
}

void change_pincode_menu(void) {
	char new_pincode[PINCODE_LENGTH], new_pincode_confirm[PINCODE_LENGTH];
	/* Authenticate first: */
	if(!authenticate("Current Pin: ")) {
		validate_authentication(0);
		return;	
	}
	
	/* Now set new pincode: */
	while(1) {
		strcpy(new_pincode, read_pin("New Pin: "));
		strcpy(new_pincode_confirm, read_pin("New Pin\nAgain: "));
		if(!strcmp(new_pincode, new_pincode_confirm)) {
			set_new_pincode(new_pincode);
			GLCD_GoTo(20,2);
			GLCD_WriteString("New pin set!", Font_System7x8);
			printf_at("Press any key to exit",0,GLCD_HEIGHT-1);
			read_key();
			return;	
		} else {
			printf_at("The pincodes do not\nmatch!\n\nPress any key to try again...",0,0);
			read_key();
		}
	}
}

void change_thermistor_type_menu(void) {
	printf("Settings > Thermistor\n_____________________\n");
	printf("> Current: %s\n\n> Select thermistor:\n1 - Analog (ADC)\n2 - Digital (I2C)", 
		((!thermistor_type())?"Analog":"Digital"));
	while(1)
		switch(getcommand()) {
			case '1': set_temperature_method(0); return;
			case '2': set_temperature_method(1); return;
			default: break;
		}
}

void settings_menu(void) {
	char option;
	char exit_settings = 0;
	
	while(!exit_settings) {
		GLCD_ClearScreen();
		GLCD_GoTo(0,0);
		printf("Menu - Settings\n_____________________\n1 - Set temperature\n2 - Set time\n3 - Change pin code\n4 - Change thermistor\n\n5 - Return");
		
		option = read_key();
		keypad_4x4_wait();
		keypad_4x4_flush();
		GLCD_ClearScreen(); 
		GLCD_GoTo(0,0);
		switch(option) {
		case '1': change_temperature_menu(); break;
		case '2': set_time_menu(); break;
		case '3': change_pincode_menu(); break;
		case '4': change_thermistor_type_menu(); break;
		case '5': exit_settings = 1; break; /* Quit settings menu */
		default: break; /* Do nothings */		
		}
		keypad_4x4_wait();
		keypad_4x4_flush();
	}
	keypad_4x4_wait();
	keypad_4x4_flush();
	GLCD_ClearScreen();
}
/*************************** Setting's Menus (END): ****************************/

/* Climate control is responsible for actually controlling the motor and managing the temperature: */
char climate_control(void) {
	int i;
	char key;
	rtc_t * rtc;

REDRAW_MENU: /* Useful for switches */

	GLCD_ClearScreen();
	printf_at("Smart Home Console\n_____________________", 0,0);
	printf_at_rl("|Time|", GLCD_WIDTH-2, GLCD_HEIGHT-2);
	printf_at(" * Temperature: 0'C\n * Desired: 0'C\n * RPM: 0    \n---------------------\n1 - Settings\n2 - Exit", 0,2);
		
	while(managing_climate) {
		/* Read temperature and adjust motor in function of temperature: */
		current_temp = thermistor_read();
		if(current_temp == 255)
			check_i2c(1); /* I2C tends to stop working unexpectedly... Maybe due to interrupts */
		
		/* Manage motor's speed: */
		if(current_temp < desired_temp - TEMP_ERROR) /* Speed left, heat it up */
			motor_spin(map(current_temp, THERMISTOR_MAX_TEMP, THERMISTOR_MIN_TEMP, MOTOR_MIN_RPM, 10000), MOTOR_LEFT);
		else if(current_temp > desired_temp + TEMP_ERROR) /* Speed right, cool it down */
			motor_spin(map(current_temp, THERMISTOR_MIN_TEMP, THERMISTOR_MAX_TEMP, MOTOR_MIN_RPM, 10000), MOTOR_RIGHT);
		else /* Temperature match */
			motor_stop();
		
		/* Display data: */
		printf_at("%d'C ", 95,2, current_temp);
		printf_at("%d'C", 70,3, desired_temp);
		printf_at("%d    ", 45, 4, motor_read_rpm());
		rtc = read_clock(); /* Read RTC */
		/* Show time: */
		printf_at_rl(" |%.2d:%.2d:%.2d|", GLCD_WIDTH-1, GLCD_HEIGHT - 1, rtc->hour, rtc->min, rtc->sec);
		for(i = 0; i < UPDATE_RATE; i++) 
			seg_update(current_temp, 10);
			
		if((key = getcommand())) {
			switch(key) {
			case '1': motor_stop(); seg_turnoff(); settings_menu(); goto REDRAW_MENU;
			case '2': motor_stop(); seg_turnoff(); managing_climate = 0; return 1;
			default: break; /* Do nothing */	
			}	
		}
	}
	motor_stop();
	seg_turnoff();
	
	return 0;
}

char request_pincode_menu(void) {
	char option;
	
	while(1) {
		GLCD_ClearScreen();
		GLCD_GoTo(0,0);
		printf("\nSmart Home Console\n_____________________\nSelect a function:\n  1 - Enter PIN\n  2 - Exit\n\nOption: ");
		keypad_4x4_wait();
		option = read_key();
		printf("%c",option);
		if(option=='1') {
			if(authenticate("Pin: ")) {
				managing_climate = 1;
				validate_authentication(1);
				return 1;
			} else {
				validate_authentication(0);
			}
		}
		else if(option=='2') {
			return 0;
		}
	}
}

extern void extra_cback(void);
extern char enable_extra;

void intro(void) {
	REDRAW_INTRO:
	GLCD_ClearScreen();
	GLCD_GoTo(0,0);
	GLCD_Rectangle(0, 0, GLCD_WIDTH+5, 5);
	GLCD_WriteString("\n\n    NG2S901\n  Assignment 2\n\n", Font_System7x8);
	printf("  Smart Home Console");
	printf_at("Press any key!", GLCD_WIDTH/2-40, GLCD_HEIGHT-1);
	GLCD_Rectangle(0, 48, GLCD_WIDTH+5, 48);
	
	/* Easter Egg: */
	enable_extra = 0;
	keypad_4x4_wait();
	while(!getcommand()) if(enable_extra) { extra_init(); goto REDRAW_INTRO; }
	
	GLCD_ClearScreen();
	GLCD_GoTo(0,0);
}

void check_i2c(char i2c_crashed) {
	/* Check if I2C is working by pinging a device: */
	if(!i2c_ping_slave(0x49) || i2c_crashed) { /* Ping Digital Thermistor */
		/* If it returns 0 then it's not working... */
		GLCD_GoTo(0,0);
		GLCD_ClearScreen();
		GLCD_WriteString("ERROR:\nI2C is not\nworking!\n\nPlease restart\nthe system", Font_System7x8);
		disable_interrupt();
		for(;;);
	}
}

extern void bluetooth_control_init(void);
extern void bluetooth_show_rpm(void);

void main(void)
{
	Initialise();
	bluetooth_control_init();
	install_cback(extra_cback, INT0_T);
	GLCD_Initialise();
	i2c_init();
	/* Before initializing RTC, check if I2C is working: */
	check_i2c(0);
	init_rtc();
	thermistor_init();
	eeprom_init();
	keyscan_4x4_init();
	init_7seg();
	init_motor();
	load_pincode(); /* Load pincode from EEPROM */
	
#if 0 /* Set this 0 to 1 to restore the default pin back, in case you forgot the pin code */
	set_new_pincode("1234");
#endif
	
	/* Load desired_temp from EEPROM: */
	desired_temp = eeprom_read(0);
	current_temp = thermistor_read();
	
	while(1) {
		intro();
		if(request_pincode_menu())
			while(1) {
				if(climate_control()) break; /* The user wanted to exit back to the intro */
				bluetooth_show_rpm();
			}
	}
}