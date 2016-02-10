#ifndef GLCD_H
#define GLCD_H
#include "font.h"

typedef struct {
	unsigned char x, y;	
} Point;

void GLCD_Initialise(void);	
void GLCD_ClearScreen(void);
void GLCD_DisplayPicture (unsigned char*);
Point GLCD_GoTo (unsigned char, unsigned char);
void GLCD_Rectangle (unsigned char, unsigned char, unsigned char, unsigned char);
void GLCD_WriteString (unsigned char*, FONT_DEF);
void GLCD_DisplayValue (unsigned long, char,FONT_DEF) ;
void GLCD_Circle(unsigned char, unsigned char, unsigned char);
void GLCD_WriteChar (unsigned char, FONT_DEF);
Point GLCD_getXY(void);

/* Extern definitions */


/* Hardware */
#define LCD_DI		P3_1  	
#define LCD_RW		P3_3  	
#define LCD_E		P3_0  	
#define LCD_CS1		P3_4  	
#define LCD_CS2		P3_5  	
#define LCD_RST		P3_2  	
#define LCD_DATA	P2    

/* LCD Registers */
#define X_ADRESS	0xB8	/* Adress base for Page 0 */
#define Y_ADRESS	0x40	/* Adress base for Y0	  */
#define START_LINE	0xC0	/* Adress base for line 0 */
#define DISPLAY_ON	0x3F	/* Turn display on	  */
#define DISPLAY_OFF	0x3E	/* Turn display off	  */

/* General use definitions */
#define RIGHT		0
#define LEFT		1
#define BUSY		0x80

#define GLCD_WIDTH 124
#define GLCD_HEIGHT 8
#define GLCD_CHAR_WIDTH 6
#define GLCD_CHARS_HORIZ (GLCD_WIDTH/GLCD_CHAR_WIDTH)
#define GLCD_CHARS_VERT (GLCD_HEIGHT/GLCD_CHAR_WIDTH)

#endif