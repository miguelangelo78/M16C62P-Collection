#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

/*
 ***********************
 LCD Driver for KS0108 
 ***********************
 */

#define KS0108_SCREEN_WIDTH	128
#define KS0108_SCREEN_HEIGHT 64

#define LCD_DISPLAY_SET_Y 0x40
#define LCD_DISPLAY_SET_X 0xB8
#define LCD_DISPLAY_START_LINE 0xC0
#define LCD_DISPLAY_ON_CMD 0x3E
#define LCD_ON 0x01
#define LCD_OFF 0x00
#define LCD_DISPLAY_STATUS_BUSY 0x80

void lcd_init(void);
void lcd_writedata(unsigned char);
void lcd_writecmd(unsigned char, unsigned char);
void lcd_cls(void);
void lcd_goto(unsigned char, unsigned char);
void lcd_writestr(char *);
void lcd_writechr(char );
unsigned char lcd_readbyte_fromrom(char *);
unsigned char lcd_read(void);
void lcd_bitmap(char *, unsigned char, unsigned char, unsigned char, unsigned char);

/* Actual graphics functions: */
void lcd_setpixel(unsigned char x, unsigned char y);
void lcd_clearpixel(unsigned char x, unsigned char y);
void lcd_rect(unsigned char x, unsigned char y, unsigned char b, unsigned char a);
void lcd_circ(unsigned char cx, unsigned char cy ,unsigned char radius);
void lcd_line(unsigned int X1,unsigned int Y1,unsigned int X2,unsigned int Y2);
void lcd_eraseline(unsigned int X1,unsigned int Y1,unsigned int X2,unsigned int Y2);

#endif