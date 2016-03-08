#ifndef KEYSCAN_4x4
#define KEYSCAN_4X4

void keyscan_4x4_init(void);

#pragma interrupt Timer_A0
void Timer_A0(void);

char getcommand(void);
char * keypad_4x4_read_buffer(void);
void keyscan_4x4_clear(void);
void keypad_4x4_wait(void);
char keypad_4x4_wait_async(void);
char read_key(void);
char * read_string(char stringLength);
char keypad_4x4_iskeypress(void);

#pragma interrupt KYB
void KYB(void);

#endif