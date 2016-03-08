#include "../init/sfr62p.h" 
#include <string.h>

#define KEYPAD_WIDTH 4
#define KEYPAD_HEIGHT 4
#define KEYPAD_BUFFER_SIZE KEYPAD_WIDTH * KEYPAD_HEIGHT
#define KEYPAD_DEBOUNCE_DELAY 40000000
#define KEYPAD_HOLD_KEY_TIMEOUT 10 /* How long the keys stay in the buffer */

char keypad_matrix[KEYPAD_HEIGHT][KEYPAD_WIDTH]
 	= { {'1', '2', '3', 'A'}, 
		{'4', '5', '6', 'B'}, 
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'} };

typedef struct {
	char debouncing; /* Is it debouncing? */
	char ready; /* Is it ready to be read? */
	char keydown; /* Is the user still pressing the key down? */
	char buffer[KEYPAD_BUFFER_SIZE];
	int buffer_size;
} keypad_4x4_t;

keypad_4x4_t keypad;

char keypad_buff_cpy[KEYPAD_BUFFER_SIZE]; /* This buffer will be returned by the function keypad_4x4_read */

void keyscan_4x4_init(void)
{
	P10D = 0x0F; /* Keypad's port (half input, half output) */
		
	/* Set up interrupts: */
	KUPIC = 0x5; /* Keypad interrupt */
	
	/* Timer interrupt: */
	TA0MR = 0b1000010; /* one shot mode */
	TA0 = 1; /* the value doesn't really matter, since we're in one shot mode */
	TA0IC = 0x04; /* timer's priority */
	enable_interrupt();
	TABSR_0 = 1; /* start timer, even if it is in one shot mode */
	
	/* Initialize everything else: */	
	keypad.debouncing = 0;
	keypad.ready = 0;
	keypad.buffer_size = 0;
	keypad.keydown = 0;
	
	memset(keypad_buff_cpy, 0, KEYPAD_BUFFER_SIZE);
}

void keypad_4x4_flush(void) 
{
	memset(keypad.buffer, 0, keypad.buffer_size);
	keypad.buffer_size = 0;
}

void keypad_delay() {
	int i;
	for(i=0;i<20;i++);
}

int hold_delay = 0;

void keyscan_4x4_clear(void) {
	memset(keypad_buff_cpy, 0, KEYPAD_BUFFER_SIZE);
}

void keypad_4x4_scan(void) 
{
	char row, col;
	/* Check if buffer is full */
	if(keypad.buffer_size > KEYPAD_BUFFER_SIZE) return;
	if(hold_delay++>KEYPAD_HOLD_KEY_TIMEOUT) {
		keyscan_4x4_clear(); /* Clear copy buffer */
		hold_delay=0;
	}
	/* NOTE: The keypad must be pulling the resistors UP. Not down */
	/* Scan keypad: */
	for(row = 0; row < KEYPAD_HEIGHT; row++) {
		P10 = ~(1 << row); /* Set only one 0 on the row */
		keypad_delay();
		for(col = 0; col < KEYPAD_WIDTH; col++) /* Scan all columns for a 0, one at a time */
			if(!(P10 & ((1<<col) << 4)) && keypad.buffer_size < KEYPAD_BUFFER_SIZE) { /* Key pressed */
				keypad.buffer[keypad.buffer_size++] = keypad_matrix[col][row]; /* Push key into buffer */
				keypad.keydown = 1;
			}
	}
	
	if(keypad.keydown) {
		keypad.ready = 1;
		keypad.keydown = 0;
	}
	
	TA0 = 40000;
	ONSF_0 = 1; /* keep calling the timer isr */	
}

#pragma interrupt Timer_A0
char ctr=0; /* This counter improves the debouncing */
void Timer_A0(void)
{
	KUPIC_3 = 0; /* clear request flag */
	if(keypad.debouncing || ctr++<2) { /* if debouncing then just wait a bit */
		keypad.debouncing = 0;
		TA0 = KEYPAD_DEBOUNCE_DELAY;
		ONSF_0 = 1;
		return;
	}
	ctr = 0;
	keypad_4x4_scan(); /* Scan multiple keys */
}

#pragma interrupt KYB
void KYB(void)
{
	if(keypad.debouncing) return; /* Ignore keypress as it is still debouncing the previous key */
	
	hold_delay = 0;
	keypad.keydown = 1;
	keypad.debouncing = 1; /* a key was pressed. we're debouncing it */
	TA0 = KEYPAD_DEBOUNCE_DELAY; /* debounce delay */
	ONSF_0 = 1; /* trigger timer 0 */
}

char * keypad_4x4_read_buffer(void) 
{
	if(keypad.ready) {
		memset(keypad_buff_cpy, 0, KEYPAD_BUFFER_SIZE); /* Clear buffer first */
		memcpy(keypad_buff_cpy, keypad.buffer, keypad.buffer_size); /* Copy keypad's buffer */
		keypad_4x4_flush(); /* Clear keypad's buffer */

		keypad.ready = 0;
	}
	return keypad_buff_cpy;	/* Return copied buffer */
}

char getcommand(void)
{
	return keypad_4x4_read_buffer()[0];
}

char keypad_4x4_iskeypress(void) {
	return keypad.keydown;
}

void keypad_4x4_wait(void) 
{
	while(1) {
		while(getcommand());
		if(!keypad_4x4_iskeypress()) /* Still pressing key */
			break;
	}
}

/* Allows the caller to wait for the keypad without blocking the main thread 
   Returns non zero if the keypad requires waiting */
char keypad_4x4_wait_async(void) {
	return getcommand();	
}

/* Same as getcommand, but blocks the thread */
char read_key(void) 
{
	char key;
	keypad_4x4_wait();
	while(!(key = getcommand()));
	return key;
}

char keypad_string_buffer[16];
char * read_string(char stringLength) {
	char i;
	memset(keypad_string_buffer, 0, 16);
	for(i = 0;i < stringLength && i < 16;i++)
		keypad_string_buffer[i] = read_key();
	return keypad_string_buffer;
}
