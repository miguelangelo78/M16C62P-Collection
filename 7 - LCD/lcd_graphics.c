#include "sfr62p.h"
#include "lcd_driver.h"

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

extern void lcd_setpixel(uint8_t x, uint8_t y);
extern void lcd_clearpixel(uint8_t x, uint8_t y);

void lcd_rect(uint8_t x, uint8_t y, uint8_t b, uint8_t a) {
	unsigned char j; 
	for (j = 0; j < a; j++) {
		lcd_setpixel(x, y + j);
		lcd_setpixel(x + b - 1, y + j);
	}
  
  	for (j = 0; j < b; j++)	{
		lcd_setpixel(x + j, y);
		lcd_setpixel(x + j, y + a - 1);
	}	
}

void lcd_circ(uint8_t cx, uint8_t cy , uint8_t radius) {
	int x, y, xchange, ychange, radiusError;
	x = radius;
	y = 0;
	xchange = 1 - 2 * radius;
	ychange = 1;
	radiusError = 0;
	while(x >= y) {
		lcd_setpixel(cx+x, cy+y); 
		lcd_setpixel(cx-x, cy+y); 
		lcd_setpixel(cx-x, cy-y);
		lcd_setpixel(cx+x, cy-y); 
		lcd_setpixel(cx+y, cy+x); 
		lcd_setpixel(cx-y, cy+x); 
		lcd_setpixel(cx-y, cy-x); 
		lcd_setpixel(cx+y, cy-x); 
		y++;
		radiusError += ychange;
		ychange += 2;
		if (radiusError*2 + xchange > 0) {
			x--;
			radiusError += xchange;
			xchange += 2;
		}
	}
}

void lcd_line(uint16_t X1, uint16_t Y1,uint16_t X2, uint16_t Y2) {
	int CurrentX, CurrentY, Xinc, Yinc;
	int Dx, Dy, TwoDx, TwoDy;
	int TwoDxAccumulatedError, TwoDyAccumulatedError;

	Dx = (X2-X1); 
	Dy = (Y2-Y1); 
	TwoDx = Dx + Dx; 
	TwoDy = Dy + Dy; 

	CurrentX = X1; 
	CurrentY = Y1; 

	Xinc = 1; 
	Yinc = 1; 

	if(Dx < 0) {
		Xinc = -1; 
		Dx = -Dx;  
		TwoDx = -TwoDx; 
	}

	if (Dy < 0)  {
		Yinc = -1; 
		Dy = -Dy; 
		TwoDy = -TwoDy;
	}

	lcd_setpixel(X1,Y1); 

	if ((Dx != 0) || (Dy != 0))  {
  		if (Dy <= Dx)  { 
    		TwoDxAccumulatedError = 0; 
    		do {
				CurrentX += Xinc; 
				TwoDxAccumulatedError += TwoDy; 
      			if(TwoDxAccumulatedError > Dx) {
        			CurrentY += Yinc; 
        			TwoDxAccumulatedError -= TwoDx; 
        		}
				lcd_setpixel(CurrentX, CurrentY);
			} while (CurrentX != X2); 
		} else {
			TwoDyAccumulatedError = 0; 
			do {
				CurrentY += Yinc; 
				TwoDyAccumulatedError += TwoDx;
				if(TwoDyAccumulatedError>Dy) {
					CurrentX += Xinc;
					TwoDyAccumulatedError -= TwoDy;
				}
				lcd_setpixel(CurrentX, CurrentY); 
			} while (CurrentY != Y2);
		}
	}
}

void lcd_eraseline(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2) {
	int CurrentX, CurrentY, Xinc, Yinc; 
	int Dx, Dy, TwoDx, TwoDy; 
	int TwoDxAccumulatedError, TwoDyAccumulatedError;

	Dx = (X2-X1); 
	Dy = (Y2-Y1); 
	TwoDx = Dx + Dx; 
	TwoDy = Dy + Dy; 

	CurrentX = X1; 
	CurrentY = Y1; 

	Xinc = 1; 
	Yinc = 1; 

	if(Dx < 0)  {
		Xinc = -1; 
		Dx = -Dx;  
		TwoDx = -TwoDx; 
	}

	if (Dy < 0) {
		Yinc = -1; 
		Dy = -Dy; 
		TwoDy = -TwoDy;
	}

	lcd_clearpixel(X1, Y1); 

	if ((Dx != 0) || (Dy != 0))  {
		if (Dy <= Dx) { 
			TwoDxAccumulatedError = 0; 
			do {
				CurrentX =CurrentX+ Xinc; 
				TwoDxAccumulatedError += TwoDy; 
				if(TwoDxAccumulatedError > Dx) {
					CurrentY += Yinc; 
					TwoDxAccumulatedError -= TwoDx; 
				}
       			lcd_clearpixel(CurrentX, CurrentY);
			} while (CurrentX != X2); 
		} else {
			TwoDyAccumulatedError = 0; 
			do {
				CurrentY += Yinc; 
				TwoDyAccumulatedError += TwoDx;
				if(TwoDyAccumulatedError>Dy) {
					CurrentX += Xinc;
					TwoDyAccumulatedError -= TwoDy;
				}
				lcd_clearpixel(CurrentX, CurrentY); 
			} while (CurrentY != Y2);
		}
	}
}