#include "sfr62p.h"
#include "delay.h"
#include "lcd_driver.h"
#include "font5x8.h"

typedef unsigned char uint8_t;

uint8_t screen_x = 0, screen_y = 0;

/******************* LCD DRIVER (BEGIN) *******************/

/* KS0108's macros: */
#define KS0108_DATA_PORT	p2
#define KS0108_DATA_DIR		pd2

#define KS0108_CTRL_PORT	p3
#define KS0108_CTRL_DIR		pd3
#define reset_gdisplay		(1 << 2)
#define KS0108_RS			(1 << 1)
#define KS0108_RW			(1 << 3)
#define KS0108_EN			(1 << 0)

#define KS0108_CS1			(1 << 4)
#define KS0108_CS2			(1 << 5)
#define KS0108_CS3			(1 << 6)

void lcd_delay(int delay) {
	for(; delay; delay--);
}

void lcd_disablecontroller(uint8_t controller) {	
	switch(controller){
		case 0 : KS0108_CTRL_PORT &= ~KS0108_CS1; break;
		case 1 : KS0108_CTRL_PORT &= ~KS0108_CS2; break;
		case 2 : KS0108_CTRL_PORT &= ~KS0108_CS3; break;
	}
}

void lcd_enablecontroller(uint8_t controller) {
	switch(controller){
		case 0 : KS0108_CTRL_PORT |= KS0108_CS1; break;
		case 1 : KS0108_CTRL_PORT |= KS0108_CS2; break;
		case 2 : KS0108_CTRL_PORT |= KS0108_CS3; break;
	}
}

uint8_t lcd_readstatus(uint8_t controller) {
	uint8_t status;
	KS0108_DATA_DIR = 0x00;
	KS0108_CTRL_PORT |= KS0108_RW;
	KS0108_CTRL_PORT &= ~KS0108_RS;
	lcd_enablecontroller(controller);
	KS0108_CTRL_PORT |= KS0108_EN;
	status = KS0108_DATA_PORT;
	KS0108_CTRL_PORT &= ~KS0108_EN;
	lcd_disablecontroller(controller);

	return status;
}

void lcd_writecmd(uint8_t commandToWrite, uint8_t controller) {
	while(((lcd_readstatus(controller) & 0x80) == 0x80));
	KS0108_DATA_DIR = 0xff;
	KS0108_CTRL_PORT &= ~(KS0108_RW | KS0108_RS);
	lcd_enablecontroller(controller);
	KS0108_DATA_PORT = commandToWrite;
	KS0108_CTRL_PORT |= KS0108_EN;
	KS0108_CTRL_PORT &= ~KS0108_EN;
	lcd_disablecontroller(controller);
}

void lcd_writedata(uint8_t dataToWrite) {
	while(lcd_readstatus(screen_x / KS0108_SCREEN_HEIGHT) & LCD_DISPLAY_STATUS_BUSY);
	KS0108_DATA_DIR = 0xff;
	KS0108_CTRL_PORT &= ~KS0108_RW;
	KS0108_CTRL_PORT |= KS0108_RS;
	KS0108_DATA_PORT = dataToWrite;
	lcd_enablecontroller(screen_x / KS0108_SCREEN_HEIGHT);
	KS0108_CTRL_PORT |= KS0108_EN;
	KS0108_CTRL_PORT &= ~KS0108_EN;
	lcd_disablecontroller(screen_x / KS0108_SCREEN_HEIGHT);
	screen_x++;
}

uint8_t lcd_read(void) {
	uint8_t tmp;
	while(lcd_readstatus(screen_x / KS0108_SCREEN_HEIGHT) & LCD_DISPLAY_STATUS_BUSY);
	KS0108_DATA_DIR = 0x00;
	KS0108_CTRL_PORT |= (KS0108_RW | KS0108_RS);
	lcd_enablecontroller(screen_x / KS0108_SCREEN_HEIGHT);
	KS0108_CTRL_PORT |= KS0108_EN;
	tmp = KS0108_DATA_PORT;
	KS0108_CTRL_PORT &= ~KS0108_EN;
	lcd_disablecontroller(screen_x / KS0108_SCREEN_HEIGHT);
	screen_x++;
	return tmp;
}

void lcd_initalizeports(void) {
	KS0108_CTRL_DIR = KS0108_DATA_DIR = 0xFF;
	KS0108_CTRL_PORT |= (KS0108_CS1 | KS0108_CS2);
	KS0108_CTRL_PORT &= ~reset_gdisplay;
	delay_us(2);
	KS0108_CTRL_PORT |= reset_gdisplay;
	delay_ms(3);
}

uint8_t lcd_readbyte_fromrom(char * ptr) {
	return *(ptr);
}

/******************* LCD DRIVER (END) *******************/

/******************* LCD DRIVER HANDLERS (BEGIN) *******************/
void lcd_init(void) {
	uint8_t i;
	lcd_initalizeports();
	for(i = 0; i < 2; i++)
		lcd_writecmd((LCD_DISPLAY_ON_CMD | LCD_ON), i);
}

void lcd_goto(unsigned char x, unsigned char y) {
	uint8_t i;
	screen_x = x;
	screen_y = y;

	for(i = 0; i < KS0108_SCREEN_WIDTH / KS0108_SCREEN_HEIGHT; i++) {
		lcd_writecmd(LCD_DISPLAY_SET_Y | 0, i);
		lcd_writecmd(LCD_DISPLAY_SET_X | y, i);
		lcd_writecmd(LCD_DISPLAY_START_LINE | 0, i);
	}
	lcd_writecmd(LCD_DISPLAY_SET_Y | (x % KS0108_SCREEN_HEIGHT), (x / KS0108_SCREEN_HEIGHT));
	lcd_writecmd(LCD_DISPLAY_SET_X | y, (x / KS0108_SCREEN_HEIGHT));
}

void lcd_cls(void) {
	uint8_t i, j;
	for(j = 0; j < KS0108_SCREEN_HEIGHT/8; j++) {
		lcd_goto(0, j);
		for(i = 0; i < KS0108_SCREEN_WIDTH; i++)
			lcd_writedata(0);
	}
}

void lcd_writechr(char chr) {
	int i;
	int j = (chr - 32) * 5; 
	for(i = 0; i < 5; i++) //changes from 5
		lcd_writedata(font5x8[j + i]); 
	lcd_writedata(0);
}

void lcd_writestr(char * str) {
	while(*str) lcd_writechr(*str++);
}

void lcd_setpixel(unsigned char x, unsigned char y) {
	uint8_t tmp;
	lcd_goto(x, (y / 8));
	tmp = lcd_read();
	lcd_goto(x, (y / 8));
	tmp = lcd_read();
	lcd_goto(x, (y / 8));
	tmp |= (1 << (y % 8));
	lcd_writedata(tmp);
}

void lcd_clearpixel(unsigned char x, unsigned char y) {
	uint8_t tmp;
	lcd_goto(x, (y / 8));
	tmp = lcd_read();
	lcd_goto(x, (y / 8));
	tmp = lcd_read();
	lcd_goto(x, (y / 8));
	tmp &= ~(1 << (y % 8));
	lcd_writedata(tmp);
}

void lcd_bitmap(char * bmp, unsigned char x, unsigned char y, unsigned char dx, unsigned char dy) {
	uint8_t i, j;
	for(j = 0; j < dy / 8; j++) {
		lcd_goto(x, y + j);
		for(i = 0; i < dx; i++) 
			lcd_writedata(lcd_readbyte_fromrom(bmp++));
	}
}

/******************* LCD DRIVER HANDLERS (END) *******************/
