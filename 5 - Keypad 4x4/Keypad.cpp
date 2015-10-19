#include "sfr62p.h"
#include <string.h>

#define enable_interrupt() _asm(" fset i");
#define disable_interrupt() _asm(" fclr i");

#define KEYPAD_WIDTH 4
#define KEYPAD_HEIGHT 4
#define KEYPAD_BUFFER_SIZE KEYPAD_WIDTH * KEYPAD_HEIGHT
#define KEYPAD_DEBOUNCE_DELAY 4000

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

void keypad_4x4_init(void) {	
	pd10 = 0x0F; /* Keypad's port (half input, half output) */
	
	/* Set up interrupts: */
	kupic = 0x5; /* Keypad interrupt */
	
	/* Timer interrupt: */
	ta0mr = 0b1000010; /* one shot mode */
	ta0 = 1; /* the value doesn't really matter, since we're in one shot mode */
	ta0ic = 0x04; /* timer's priority */
	enable_interrupt();
	ta0s = 1; /* start timer, even if it is in one shot mode */
	
	/* Initialize everything else: */	
	keypad.debouncing = 0;
	keypad.ready = 0;
	keypad.buffer_size = 0;
	keypad.keydown = 0;
	
	memset(keypad_buff_cpy, 0, KEYPAD_BUFFER_SIZE);
}

void keypad_4x4_flush(void) {
	int i, j;
	for(i = 0; i < keypad.buffer_size; i++)
		keypad.buffer[i] = 0;
	keypad.buffer_size = 0;
}

void keypad_4x4_scan(void) {
	int row, col;

	/* Check if buffer is full */
	if(keypad.buffer_size > KEYPAD_BUFFER_SIZE) return;
	
	/* NOTE: The keypad must be pulling the resistors UP. Not down */
	/* Scan keypad: */
	for(row = 0; row < KEYPAD_HEIGHT; row++) {
		p10 = ~((1 << row)) & 0x0F; /* Set only one 1 on the row */
		for(col = 0; col < KEYPAD_WIDTH; col++) /* Scan all columns for a 1, one at a time */
			if(!(p10 & ((1<<col) << 4)) && keypad.buffer_size < KEYPAD_BUFFER_SIZE) { /* Key pressed */
				keypad.buffer[keypad.buffer_size++] = keypad_matrix[col][row]; /* Push key into buffer */
				keypad.keydown = 1;
			}
	}
	
	if(keypad.keydown) {
		keypad.ready = 1;
		keypad.keydown = 0;
		ta0 = 25;
	}
	
	ta0os = 1; /* keep calling the timer isr */	
}

#pragma interrupt TimerA0(vect=21)
void TimerA0(void) {
	ir_kupic = 0; /* clear request flag */
	if(keypad.debouncing) { /* if debouncing then just wait a bit */
		keypad.debouncing = 0;
		ta0 = KEYPAD_DEBOUNCE_DELAY;
		ta0os = 1;
		return;
	}
	keypad_4x4_scan(); /* Scan multiple keys */	
}

#pragma interrupt KeypadISR(vect=13)
void KeypadISR(void) {
	keypad.keydown = 1;
	keypad.debouncing = 1; /* a key was pressed. we're debouncing it */
	ta0 = 25; /* debounce delay */
	ta0os = 1; /* trigger timer 0 */
}

char * keypad_4x4_read_buffer(void) {
	if(keypad.ready) {
		memset(keypad_buff_cpy, 0, KEYPAD_BUFFER_SIZE); /* Clear buffer first */
		memcpy(keypad_buff_cpy, keypad.buffer, keypad.buffer_size); /* Copy keypad's buffer */
		keypad_4x4_flush(); /* Clear keypad's buffer */

		keypad.ready = 0;
	}
	return keypad_buff_cpy;	/* Return copied buffer */
}

char keypad_4x4_read(void) {
	return keypad_4x4_read_buffer()[0];
}

void main(void) {
	keypad_4x4_init();
	
	pd5 = 0xFF;
	for(;;) p5 = keypad_4x4_read();
}
