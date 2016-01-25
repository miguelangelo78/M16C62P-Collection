#include "sfr62p.h"

void Initialise(void)
{    
	unsigned short count = 10000;

	PRCR_0 = 1;
    CM1 = 0X20;
	CM0_6 = 0;							
	PRCR_1 = 1;					
	PM20 = 1;					
	PRCR_1 = 0;					
    PLC0 = 0X12;
	PLC07 = 1;					
    while(count > 0)
	{
		count--;
	}
    CM11 = 1;
	PRCR_0 = 0;   				
	PRCR_0 = 1;
   	P8D_7 = 0;		
   	P8D_6 = 0;
   	CM0_4 = 1; 		
	PRCR_1 = 1;
	PM10 = 1;		
	PRCR_1 = 0;
	P0=P1=P2=P3=P4=P5=P6=P7=P9=P10=0X0;       
	P4= 0X0F;
	P1D=P2D=P3D=P4D=P5D=P6D=0xFF;
	PRCR_2=1;
	P9D=0xFF;
	PRCR_2 =0;
	P6D |= 0x0F;
	P8_0 = P8_1 = 0;
	P8D_0 = P8D_1 = 1;	
	P3D=0xFF;
	
	TA0MR=0x80;
	TA0=20000;
	TA0IC=0x04;
	
	enable_interrupt();
	
	TABSR=0x07;
	
	P8D=0x00;		// all pins input switches connected to P8_2,P8_3,P8_4
}
