#ifndef FONT_H
#define FONT_H

typedef struct FONT_DEF{
	unsigned char u8Width; /* Character width for storage         */
	unsigned char u8Height; /* Character height for storage        */
	unsigned char *au8FontTable; /* Font table start address in memory  */
} FONT_DEF;

extern FONT_DEF Font_System5x8;
extern FONT_DEF Font_System7x8;

extern unsigned char FontSystem3x6[195];
extern unsigned char FontSystem5x8[485];
extern unsigned char FontSystem7x8[679];

#endif