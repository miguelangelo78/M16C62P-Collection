#include "../init/sfr62p.h" 
#include "glcd.h"

FONT_DEF Font_System3x6  = {3, 6,FontSystem3x6};
FONT_DEF Font_System5x8  = {5, 8, FontSystem5x8};
FONT_DEF Font_System7x8  = {7, 8, FontSystem7x8};

/* Local functions prototypes */
void LcdSelectSide(unsigned char);
void LcdDataWrite (unsigned char);
void LcdInstructionWrite (unsigned char);
unsigned char LcdDataRead (void);
void LcdSetDot (unsigned char, unsigned char);
void LcdWaitBusy (void);
void LcdDelay (unsigned long);
//void GLCD_WriteChar (unsigned char, FONT_DEF);

/* Global variables */
unsigned char u8CursorX, u8CursorY;

/*******************************************************************************/
/* 			        PROGRAM CODE				       */
/*******************************************************************************/

/*-------------------------------------------------------------------------------
Tempo for the LCD timing
	pause()
-------------------------------------------------------------------------------*/
void LcdDelay(unsigned long u32Duration)
{
	unsigned long u32Delay;
	for (u32Delay=0; u32Delay<u32Duration; u32Delay++);
}

/*-------------------------------------------------------------------------------
LCD Initialization
	GLCD_LcdINI""T()
-------------------------------------------------------------------------------*/
void GLCD_Initialise(void)	
{
	LCD_DATA = 0;
	LCD_DI = 0;
  	LCD_RW = 0;
  	LCD_E = 0;
  	LCD_CS1 = 0;
  	LCD_CS2 = 0;
  	
  	LCD_RST = 1;
  	LcdDelay(10);
  	LCD_RST=0;
  	LcdDelay(10);
  	LCD_RST=1;
  	  	
  	LcdSelectSide(LEFT);
  	LcdInstructionWrite(DISPLAY_OFF); /* Display OFF */
  	LcdInstructionWrite(START_LINE);
  	LcdInstructionWrite(X_ADRESS);
  	LcdInstructionWrite(Y_ADRESS);
  	LcdInstructionWrite(DISPLAY_ON); /* Display ON */
  	
  	LcdSelectSide(RIGHT);
  	LcdInstructionWrite(DISPLAY_OFF); /* Display OFF */
  	LcdInstructionWrite(START_LINE);
  	LcdInstructionWrite(X_ADRESS);
  	LcdInstructionWrite(Y_ADRESS);
  	LcdInstructionWrite(DISPLAY_ON); /* Display ON */
  	
  	GLCD_ClearScreen();
}  	

/*-------------------------------------------------------------------------------
Select the side of the LCD
	LcdSelectSide(unsigned char u8LcdSide)
		u8LcdSide = LEFT or RIGHT
-------------------------------------------------------------------------------*/
void LcdSelectSide(unsigned char u8LcdSide)
{
	if(u8LcdSide == RIGHT)
	{
		/* switch to right */
		LCD_E=0;
		LCD_DI=0;
              	LCD_RW=1;
              	LCD_CS1=0; 	
              	LCD_CS2=1;
	
              	LcdInstructionWrite(Y_ADRESS); /* Set column to 0 */
        }
	else
	{
		/* switch to left */
		LCD_E=0;
		LCD_DI=0;
              	LCD_RW=1;
              	LCD_CS1=1; 	
              	LCD_CS2=0;
	           
              	LcdInstructionWrite(Y_ADRESS); /* Set column to 0 */
	}
}

/*-------------------------------------------------------------------------------
Send datas to the LCD
	LcdDataWrite (unsigned char u8Data)
		u8Data = data to send to the LCD
-------------------------------------------------------------------------------*/
void LcdDataWrite (unsigned char u8Data)
{
	LcdWaitBusy ();		/* wait until LCD not busy */
	LCD_DI = 1;      	/* Data mode */
	LCD_RW = 0;      	/* write mode */
	
	LCD_DATA = u8Data;	/* outbyte */
	
	LCD_E = 1;		/* Strobe */
	LcdDelay(1);	
	LCD_E = 0;
}

/*-------------------------------------------------------------------------------
Send instruction to the LCD
	LcdDataWrite (unsigned char u8Instruction)
		u8Instruction = Instructino to send to the LCD
-------------------------------------------------------------------------------*/
void LcdInstructionWrite (unsigned char u8Instruction)
{
   	LcdWaitBusy ();		/* wait until LCD not busy */
   	LCD_DI = 0;       	/* Instruction mode */
  	LCD_RW = 0;       	/* Write mode */
	 	
  	LCD_DATA = u8Instruction;/* outbyte */
  	  	
  	LCD_E = 1;		/* Strobe */
  	LcdDelay(1);
  	LCD_E = 0;
}

/*-------------------------------------------------------------------------------
Read data from LCD
	unsigned char = LcdDataRead();
-------------------------------------------------------------------------------*/
unsigned char LcdDataRead ()
{
	LCD_DATA = 0xFF;	/* set LCD_DATA port in input mode */
	
  	LCD_RW = 1;       	/* Read mode */
  	LCD_DI = 1;       	/* Data mode */
  	
  	LCD_E  = 0;       	/* strobe */
  	LcdDelay(1);
	LCD_E  = 1;
	LcdDelay(1);
	  	  	
	return LCD_DATA;	/* return the data read */
}

/*-------------------------------------------------------------------------------
Wait as long as the LCD is busy
	LcdWaitBusy();
-------------------------------------------------------------------------------*/
void LcdWaitBusy ()
{
	LCD_DATA=0xFF;		/* set LCD_DATA port in input mode */
	  	
  	LCD_DI = 0;       	/* Instruction mode */
  	LCD_RW = 1;       	/* Read mode */
  	
  	LCD_E  = 0;       	/* strobe */
  	LcdDelay(1);
  	LCD_E  = 1;
	
	while (LCD_DATA & 0x7F == BUSY); /* mask the other status bits and test the
					    BUSY bit */
}

/*-------------------------------------------------------------------------------
Clear the LCD screen
	GLCD_ClearScren ()
-------------------------------------------------------------------------------*/
void GLCD_ClearScreen(void)
{
  	 unsigned char u8Page=0;
  	 unsigned char u8Column=0;
    
    	/* process the 8 pages of the LCD */
  	for (u8Page = 0; u8Page < 8; u8Page++) 
    	{
      		LcdSelectSide(LEFT);			/* Select left side */
      		LcdInstructionWrite(X_ADRESS | u8Page); /* Set the page number */
      		LcdInstructionWrite(Y_ADRESS);		/* Set column to 0 */
      		
      		/* process a page on both sides */
      		for (u8Column = 0; u8Column < 128; u8Column++) 
		{
	  		if (u8Column == 64)
            			{    
              			LcdSelectSide(RIGHT);	/* Select right side */
              			LcdInstructionWrite(X_ADRESS | u8Page); /* Set the page number */
              			LcdInstructionWrite(Y_ADRESS); /* Set column to 0 */
      		             	}
          		LcdDataWrite (0x00);		/* erase a column */
		}
    	}
}

/*-------------------------------------------------------------------------------
Send an image to the LCD
	GLCD_DisplayPicture (unsigned char *au8PictureData)
		au8PictureData = contains datas for picture
-------------------------------------------------------------------------------*/
void GLCD_DisplayPicture (unsigned char *au8PictureData)
{
	 unsigned char u8Page=0;
  	 unsigned char u8Column=0;
      
        /* Send the left side */
        LcdSelectSide(LEFT);
  	for (u8Page = 0; u8Page < 8; u8Page++) /* loop on the 8 pages */
    	{
      		LcdInstructionWrite(X_ADRESS | u8Page); /* Set the page */
      		for (u8Column = 0; u8Column < 64; u8Column++)
			LcdDataWrite(au8PictureData[(128*u8Page)+u8Column]);
	}
	
	/* Send the right side */
	LcdSelectSide(RIGHT);
  	for (u8Page = 0; u8Page < 8; u8Page++) /* loop on the 8 pages */
    	{
      		LcdInstructionWrite(X_ADRESS | u8Page); /* Set the page */
      		for (u8Column = 64; u8Column < 128; u8Column++)
			LcdDataWrite(au8PictureData[(128*u8Page)+u8Column]);
	}	
}

/*-------------------------------------------------------------------------------
Draw a dot on the LCD
	LcdSetDoc (unsigned char u8Xaxis, unsigned char u8Yaxis)
		u8Xaxis = absciss
		u8Yaxis = ordinate
-------------------------------------------------------------------------------*/	
void LcdSetDot (unsigned char u8Xaxis, unsigned char u8Yaxis)
{
 	 unsigned char u8DataRead=0;
 	
 	LcdInstructionWrite(START_LINE); /* Set adress for line 0 */

	/* Left side */
	if (u8Xaxis < 64)
	{
    		LcdSelectSide (LEFT);				/* Select left side */
    		LcdInstructionWrite (X_ADRESS + (u8Yaxis / 8)); /* Select page number */
    		LcdInstructionWrite (Y_ADRESS + u8Xaxis);	/* Select column */
    		
    		u8DataRead = LcdDataRead ();  			/* read the current location */
    		u8DataRead = LcdDataRead ();  			/* on the LCD. dummy read */
    		
    		LcdInstructionWrite (X_ADRESS + (u8Yaxis / 8)); /* Select page number */
    		LcdInstructionWrite (Y_ADRESS + u8Xaxis);	/* Select column */
    		LcdDataWrite (u8DataRead | (1 << (u8Yaxis % 8)));/* plot the dot */
    	} 
  	else 
  	/* Right side */
  	{
    		LcdSelectSide(RIGHT);				/* Select left side */         
    		LcdInstructionWrite (X_ADRESS + (u8Yaxis / 8)); /* Select page number */       
    		LcdInstructionWrite (Y_ADRESS + u8Xaxis - 64);  /* Select column */            
    		                                                                               
    		u8DataRead = LcdDataRead ();                    /* read the current location */
    		u8DataRead = LcdDataRead ();                    /* on the LCD. dummy read */   
    		                                                                               
    		LcdInstructionWrite (X_ADRESS + (u8Yaxis / 8)); /* Select page number */       
    		LcdInstructionWrite (Y_ADRESS + u8Xaxis - 64);  /* Select column */            
    		LcdDataWrite (u8DataRead | (1 << (u8Yaxis % 8)));/* plot the dot */   
    		
    	}
      	LcdInstructionWrite(START_LINE); /* Set adress for line 0 */
}

/*-------------------------------------------------------------------------------
Print a char on the LCD
	GLCD_Putchar (unsigned char u8Char)
		u8Char = char to display
-------------------------------------------------------------------------------*/
void GLCD_WriteChar (unsigned char u8Char, FONT_DEF toto)
{
	unsigned char u8CharColumn=0,u8RightSide=0;
	unsigned char u8UpperCharPointer=1,u8Page=0;
	
	/* TODO: Add scroll function */
	
	/* Newline and carriage return: */
	if(u8Char == '\n') {
		u8CursorX = 0;
		u8CursorY++;
		if (u8CursorY == 8) u8CursorY = 0;
		return;	
	}
	if(u8Char == '\r') {
		u8CursorX = 0;
		if (u8CursorY == 8) u8CursorY = 0;
		return;	
	}
	
	/* test for carrier return */
	if (u8CursorX > 128 - (toto.u8Width) ) 
	{	
		u8CursorX = 0;
		u8CursorY++;
		if (u8CursorY == 8) u8CursorY = 0;
	}
	
	/* Select the side of the LCD */
	if (u8CursorX < 64)
	{	
		/* Select left side */
		LcdSelectSide (LEFT);
		LcdInstructionWrite (Y_ADRESS + u8CursorX);
	}
	else 
	{
		/* Select right side */
		LcdSelectSide (RIGHT);
		LcdInstructionWrite (Y_ADRESS + u8CursorX - 64);
	}
	    	
    /* Draw a char */
	while (u8CharColumn < (toto.u8Width) )
	{
		if ((toto.u8Height) > 8)
		{
			u8UpperCharPointer = 2;
			if (u8CursorX > 64)
				u8RightSide=64;
			LcdInstructionWrite (X_ADRESS + u8CursorY -1); 
			LcdInstructionWrite (Y_ADRESS + u8CursorX -u8RightSide);
			LcdDataWrite ((toto.au8FontTable)[( (u8Char - 32) * u8UpperCharPointer * (toto.u8Width) ) + ((u8CharColumn * u8UpperCharPointer) + 1)]);
			LcdInstructionWrite (Y_ADRESS + u8CursorX -u8RightSide);
			
		}
		LcdInstructionWrite (X_ADRESS + u8CursorY); 
		LcdDataWrite ((toto.au8FontTable)[( (u8Char - 32) * u8UpperCharPointer * (toto.u8Width) ) + (u8CharColumn * u8UpperCharPointer)]);		
		u8CharColumn++;
		u8CursorX++;
		
		/* test if a char is wrote on both sides of LCD */
		if (u8CursorX == 64)
		{
			u8RightSide=64;
			LcdSelectSide (RIGHT);	/* if yes, select right side */
			LcdInstructionWrite (X_ADRESS + u8CursorY);
			LcdInstructionWrite (Y_ADRESS + u8CursorX -u8RightSide);
		}
	}
	
	/* Insert a space after a char */	
	if (u8CursorX < 128) 		/* Check if this is the last char of the line */
	{
		if ((toto.u8Height) > 8)
		{
			LcdInstructionWrite (X_ADRESS + u8CursorY - 1); /* Select the page of the LCD */
			LcdInstructionWrite (Y_ADRESS + u8CursorX);
			LcdDataWrite(0x00);
			LcdInstructionWrite (Y_ADRESS + u8CursorX -u8RightSide);
		}
		LcdInstructionWrite (X_ADRESS + u8CursorY); /* Select the page of the LCD */
		LcdInstructionWrite (Y_ADRESS + u8CursorX );	
		LcdDataWrite(0x00); 	/* if not then insert a space before next letter */
	}
			
	u8CharColumn++;
	u8CursorX++;
}

/*-------------------------------------------------------------------------------
Print a string on the LCD
	GLCD_Printf (unsigned char *au8Text) 
		*au8Text = string to display
-------------------------------------------------------------------------------*/
void GLCD_WriteString (unsigned char *au8Text, FONT_DEF toto) 
{
	while(*au8Text != 0)
	{
		GLCD_WriteChar (*au8Text,toto);
		au8Text++;
	}
}

/*-------------------------------------------------------------------------------
Set the position of the cursor
	GLCD_Locate (unsigned char u8Column, unsigned char u8Line)
		u8Column = Column number of the cursor (in pixels)
		u8Line   = Line number of the cursor (in page number)
-------------------------------------------------------------------------------*/
Point GLCD_GoTo(unsigned char u8Column, unsigned char u8Line)
{
	Point oldPoint;
	oldPoint.x = u8CursorX;
	oldPoint.y = u8CursorY;
	u8CursorX = u8Column;
	u8CursorY = u8Line;
	return oldPoint;
}

/*-------------------------------------------------------------------------------
Convert a digital value into an ASCII string
	GLCD_DisplayValue (unsigned long u32Value, unsigned char u8NumberOfIntegerDigits, unsigned char u8NumberOfDecimalDigits)
		u32Value = 32 bits Value to display
		u8NumberOfIntegerDigits = Number of interger digits.
					  If this number is higher than the number of integer
					  digits, it will display the non significant '0'
		u8NumberOfDecimalDigits = Number of decimal digits			
-------------------------------------------------------------------------------*/
void GLCD_DisplayValue (unsigned long Number,  char Digits, FONT_DEF toto) 
{
	 char Size_of_String=0; 	/* Size of the table */
	 unsigned char Number_String[10];	/* String where is converted the value */
	 char Position=0;					/* Character number in the string */
	 char tt=0;
		Size_of_String=Digits;
		Number_String[Size_of_String] = 0;
		Size_of_String=Size_of_String-1;
		for(Position=0 ;Position<=Size_of_String;Position++)
		{
		
			Number_String[Size_of_String-Position] = Number % 10 + '0';
			Number /= 10;
		}
	/* Display the string */
	GLCD_WriteString(Number_String,toto);
}	

/*-------------------------------------------------------------------------------
Draw a rectangle on the LCD
	GLCD_Rectangle (unsigned char u8Xaxis1,unsigned char u8Yaxis1,unsigned char u8Xaxis2,unsigned char u8Yaxis2)
		u8Xaxis1 = absciss top-left (in pixels)
		u8Yaxis1 = ordinate top-left (in pixels)
		u8Xaxis2 = absciss bottom-right (in pixels)
		u8Yaxis2 = ordinate bottom-right (in pixels)
-------------------------------------------------------------------------------*/
void GLCD_Rectangle (unsigned char u8Xaxis1,unsigned char u8Yaxis1,unsigned char u8Xaxis2,unsigned char u8Yaxis2)
{
  	 unsigned char u8CurrentValue=0;

	/* Draw the two horizontal lines */
  	for (u8CurrentValue = 0; u8CurrentValue < u8Xaxis2 - u8Xaxis1+ 1; u8CurrentValue++) 
  	{
		LcdSetDot(u8Xaxis1 + u8CurrentValue, u8Yaxis1);
		LcdSetDot(u8Xaxis1 + u8CurrentValue, u8Yaxis2);
	}
  	
  	/* draw the two vertical lines */
  	for (u8CurrentValue = 0; u8CurrentValue < u8Yaxis2 - u8Yaxis1 + 1; u8CurrentValue++)	
  	{
		LcdSetDot(u8Xaxis1, u8Yaxis1 + u8CurrentValue);
		LcdSetDot(u8Xaxis2, u8Yaxis1 + u8CurrentValue);
	}
}

/*-------------------------------------------------------------------------------
Draw a circle on the LCD
	GLCD_Circle (unsigned char u8CenterX, unsigned char u8CenterY, unsigned char u8Radius)
		u8CenterX = Center absciss (in pixels)
		u8CenterY = Center ordinate (in pixels) 
		u8Radius  = Radius (in pixels)
-------------------------------------------------------------------------------*/
void GLCD_Circle (unsigned char u8CenterX, unsigned char u8CenterY, unsigned char u8Radius)
{
	 int s16tswitch=0, s16y=0, s16x=0;
	 unsigned char u8d;

	u8d = u8CenterY - u8CenterX;
	s16y = u8Radius;
	s16tswitch = 3 - 2 * u8Radius;
	while (s16x <= s16y) 
	{
		LcdSetDot(u8CenterX + s16x, u8CenterY + s16y);
		LcdSetDot(u8CenterX + s16x, u8CenterY - s16y);
		
		LcdSetDot(u8CenterX - s16x, u8CenterY + s16y);
		LcdSetDot(u8CenterX - s16x, u8CenterY - s16y);
		
		LcdSetDot(u8CenterY + s16y - u8d, u8CenterY + s16x);
		LcdSetDot(u8CenterY + s16y - u8d, u8CenterY - s16x);
		LcdSetDot(u8CenterY - s16y - u8d, u8CenterY + s16x); 
		LcdSetDot(u8CenterY - s16y - u8d, u8CenterY - s16x);

		if (s16tswitch < 0) 
			s16tswitch += (4 * s16x + 6);
		else 
		{
			s16tswitch += (4 * (s16x - s16y) + 10);
			s16y--;
		}
	
		s16x++;
	}
}

Point GLCD_getXY(void) {
	Point p;
	p.x = u8CursorX;
	p.y = u8CursorY;
	return p;
}
