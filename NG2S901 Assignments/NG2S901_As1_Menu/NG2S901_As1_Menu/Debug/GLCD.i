#line 1 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/GLCD.C"

#line 1 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/../init/sfr62p.h"
 







 





 















#pragma ADDRESS		PM0_ADDR	0004H			 

#pragma ADDRESS		PM1_ADDR	0005H			 

#pragma ADDRESS		CM0_ADDR	0006H			 

#pragma ADDRESS		CM1_ADDR	0007H			 

#pragma ADDRESS		CSR_ADDR	0008H			 

#pragma ADDRESS		AIER_ADDR	0009H			 

#pragma ADDRESS		PRCR_ADDR	000AH			 

#pragma ADDRESS		DBR_ADDR	000BH			 

#pragma ADDRESS		CM2_ADDR	000CH			 

#pragma ADDRESS		CM3_ADDR	000DH			 

#pragma ADDRESS		WDTS_ADDR	000EH			 

#pragma ADDRESS		WDC_ADDR	000FH			 

#pragma ADDRESS		RMAD0_ADDR	0010H			 

#pragma ADDRESS		RMAD1_ADDR	0014H			 

#pragma ADDRESS		CSE_ADDR	001BH			 

#pragma ADDRESS		PLC0_ADDR	001CH			 

#pragma ADDRESS		PM2_ADDR	001EH			 

#pragma ADDRESS		SAR0_ADDR	0020H			 

#pragma ADDRESS		DAR0_ADDR	0024H			 

#pragma ADDRESS		TCR0_ADDR	0028H			 

#pragma ADDRESS		DM0CON_ADDR 002CH			 

#pragma ADDRESS		SAR1_ADDR	0030H			 

#pragma ADDRESS		DAR1_ADDR	0034H			 

#pragma ADDRESS		TCR1_ADDR	0038H			 

#pragma ADDRESS		DM1CON_ADDR 003CH			 

#pragma ADDRESS		INT3IC_ADDR 0044H			 

#pragma ADDRESS		TB5IC_ADDR	0045H			 

#pragma ADDRESS		TB4IC_ADDR	0046H			 

#pragma ADDRESS		TB3IC_ADDR	0047H			 

#pragma ADDRESS		S4IC_ADDR	0048H			 

#pragma ADDRESS		S3IC_ADDR	0049H			 

#pragma ADDRESS		INT5IC_ADDR 0048H			 

#pragma ADDRESS		INT4IC_ADDR 0049H			 

#pragma ADDRESS		BCNIC_ADDR	004AH			 

#pragma ADDRESS		DM0IC_ADDR	004BH			 

#pragma ADDRESS		DM1IC_ADDR	004CH			 

#pragma ADDRESS		KUPIC_ADDR	004DH			 

#pragma ADDRESS		ADIC_ADDR	004EH			 

#pragma ADDRESS		S2TIC_ADDR	004FH			 

#pragma ADDRESS		S2RIC_ADDR	0050H			 

#pragma ADDRESS		S0TIC_ADDR	0051H			 

#pragma ADDRESS		S0RIC_ADDR	0052H			 

#pragma ADDRESS		S1TIC_ADDR	0053H			 

#pragma ADDRESS		S1RIC_ADDR	0054H			 

#pragma ADDRESS		TA0IC_ADDR	0055H			 

#pragma ADDRESS		TA1IC_ADDR	0056H			 

#pragma ADDRESS		TA2IC_ADDR	0057H			 

#pragma ADDRESS		TA3IC_ADDR	0058H			 

#pragma ADDRESS		TA4IC_ADDR	0059H			 

#pragma ADDRESS		TB0IC_ADDR	005AH			 

#pragma ADDRESS		TB1IC_ADDR	005BH			 

#pragma ADDRESS		TB2IC_ADDR	005CH			 

#pragma ADDRESS		INT0IC_ADDR 005DH			 

#pragma ADDRESS		INT1IC_ADDR 005EH			 

#pragma ADDRESS		INT2IC_ADDR 005FH			 

#pragma ADDRESS		FMR1_ADDR	01B5H			 

#pragma ADDRESS		FMR0_ADDR	01B7H			 

#pragma ADDRESS		RMAD2_ADDR	01B8H			 

#pragma ADDRESS		AIER2_ADDR	01BBH			 

#pragma ADDRESS		RMAD3_ADDR	01BCH			 

#pragma ADDRESS		PCLKR_ADDR	025EH			 

#pragma ADDRESS		TBSR_ADDR	0340H			 

#pragma ADDRESS		TA11_ADDR	0342H			 

#pragma ADDRESS		TA21_ADDR	0344H			 

#pragma ADDRESS		TA41_ADDR	0346H			 

#pragma ADDRESS		INVC0_ADDR	0348H			 

#pragma ADDRESS		INVC1_ADDR	0349H			 

#pragma ADDRESS		IDB0_ADDR	034AH			 

#pragma ADDRESS		IDB1_ADDR	034BH			 

#pragma ADDRESS		DTT_ADDR	034CH			 

#pragma ADDRESS		ICTB2_ADDR	034DH			 

#pragma ADDRESS		TB3_ADDR	0350H			 

#pragma ADDRESS		TB4_ADDR	0352H			 

#pragma ADDRESS		TB5_ADDR	0354H			 

#pragma ADDRESS		TB3MR_ADDR	035BH			 

#pragma ADDRESS		TB4MR_ADDR	035CH			 

#pragma ADDRESS		TB5MR_ADDR	035DH			 

#pragma ADDRESS		IFSR2A_ADDR	035EH			 

#pragma ADDRESS		IFSR_ADDR	035FH			 

#pragma ADDRESS		S3TRR_ADDR	0360H			 

#pragma ADDRESS		S3C_ADDR	0362H			 

#pragma ADDRESS		S3BRG_ADDR	0363H			 

#pragma ADDRESS		S4TRR_ADDR	0364H			 

#pragma ADDRESS		S4C_ADDR	0366H			 

#pragma ADDRESS		S4BRG_ADDR	0367H			 

#pragma ADDRESS		U0SMR4_ADDR	036CH			 

#pragma ADDRESS		U0SMR3_ADDR	036DH			 

#pragma ADDRESS		U0SMR2_ADDR	036EH			 

#pragma ADDRESS		U0SMR_ADDR	036FH			 

#pragma ADDRESS		U1SMR4_ADDR	0370H			 

#pragma ADDRESS		U1SMR3_ADDR	0371H			 

#pragma ADDRESS		U1SMR2_ADDR	0372H			 

#pragma ADDRESS		U1SMR_ADDR	0373H			 

#pragma ADDRESS		U2SMR4_ADDR	0374H			 

#pragma ADDRESS		U2SMR3_ADDR 0375H			 

#pragma ADDRESS		U2SMR2_ADDR 0376H			 

#pragma ADDRESS		U2SMR_ADDR	0377H			 

#pragma ADDRESS		U2MR_ADDR	0378H			 

#pragma ADDRESS		U2BRG_ADDR	0379H			 

#pragma ADDRESS		U2TB_ADDR	037AH			 

#pragma ADDRESS		U2C0_ADDR	037CH			 

#pragma ADDRESS		U2C1_ADDR	037DH			 

#pragma ADDRESS		U2RB_ADDR	037EH			 

#pragma ADDRESS		TABSR_ADDR	0380H			 

#pragma ADDRESS		CPSRF_ADDR	0381H			 

#pragma ADDRESS		ONSF_ADDR	0382H			 

#pragma ADDRESS		TRGSR_ADDR	0383H			 

#pragma ADDRESS		UDF_ADDR	0384H			 

#pragma ADDRESS		TA0_ADDR	0386H			 

#pragma ADDRESS		TA1_ADDR	0388H			 

#pragma ADDRESS		TA2_ADDR	038AH			 

#pragma ADDRESS		TA3_ADDR	038CH			 

#pragma ADDRESS		TA4_ADDR	038EH			 

#pragma ADDRESS		TB0_ADDR	0390H			 

#pragma ADDRESS		TB1_ADDR	0392H			 

#pragma ADDRESS		TB2_ADDR	0394H			 

#pragma ADDRESS		TA0MR_ADDR	0396H			 

#pragma ADDRESS		TA1MR_ADDR	0397H			 

#pragma ADDRESS		TA2MR_ADDR	0398H			 

#pragma ADDRESS		TA3MR_ADDR	0399H			 

#pragma ADDRESS		TA4MR_ADDR	039AH			 

#pragma ADDRESS		TB0MR_ADDR	039BH			 

#pragma ADDRESS		TB1MR_ADDR	039CH			 

#pragma ADDRESS		TB2MR_ADDR	039DH			 

#pragma ADDRESS		TB2SC_ADDR	039EH			 

#pragma ADDRESS		U0MR_ADDR	03A0H			 

#pragma ADDRESS		U0BRG_ADDR	03A1H			 

#pragma ADDRESS		U0TB_ADDR	03A2H			 

#pragma ADDRESS		U0C0_ADDR	03A4H			 

#pragma ADDRESS		U0C1_ADDR	03A5H			 

#pragma ADDRESS		U0RB_ADDR	03A6H			 

#pragma ADDRESS		U1MR_ADDR	03A8H			 

#pragma ADDRESS		U1BRG_ADDR	03A9H			 

#pragma ADDRESS		U1TB_ADDR	03AAH			 

#pragma ADDRESS		U1C0_ADDR	03ACH			 

#pragma ADDRESS		U1C1_ADDR	03ADH			 

#pragma ADDRESS		U1RB_ADDR	03AEH			 

#pragma ADDRESS		UCON_ADDR	03B0H			 

#pragma ADDRESS		FIDR_ADDR	03B4H			 

#pragma ADDRESS		DM0SL_ADDR	03B8H			 

#pragma ADDRESS		DM1SL_ADDR	03BAH			 

#pragma ADDRESS		CRCD_ADDR	03BCH			 

#pragma ADDRESS		CRCIN_ADDR	03BEH			 

#pragma ADDRESS		AD0_ADDR	03C0H			 

#pragma ADDRESS		AD1_ADDR	03C2H			 

#pragma ADDRESS		AD2_ADDR	03C4H			 

#pragma ADDRESS		AD3_ADDR	03C6H			 

#pragma ADDRESS		AD4_ADDR	03C8H			 

#pragma ADDRESS		AD5_ADDR	03CAH			 

#pragma ADDRESS		AD6_ADDR	03CCH			 

#pragma ADDRESS		AD7_ADDR	03CEH			 

#pragma ADDRESS		ADCON2_ADDR 03D4H			 

#pragma ADDRESS		ADCON0_ADDR 03D6H			 

#pragma ADDRESS		ADCON1_ADDR 03D7H			 

#pragma ADDRESS		DA0_ADDR	03D8H			 

#pragma ADDRESS		DA1_ADDR	03DAH			 

#pragma ADDRESS		DACON_ADDR	03DCH			 

#pragma ADDRESS		PC14_ADDR	03DEH			 

#pragma ADDRESS		PUR3_ADDR	03DFH			 

#pragma ADDRESS		P0_ADDR		03E0H			 

#pragma ADDRESS		P1_ADDR		03E1H			 

#pragma ADDRESS		P0D_ADDR	03E2H			 

#pragma ADDRESS		P1D_ADDR	03E3H			 

#pragma ADDRESS		P2_ADDR		03E4H			 

#pragma ADDRESS		P3_ADDR		03E5H			 

#pragma ADDRESS		P2D_ADDR	03E6H			 

#pragma ADDRESS		P3D_ADDR	03E7H			 

#pragma ADDRESS		P4_ADDR		03E8H			 

#pragma ADDRESS		P5_ADDR		03E9H			 

#pragma ADDRESS		P4D_ADDR	03EAH			 

#pragma ADDRESS		P5D_ADDR	03EBH			 

#pragma ADDRESS		P6_ADDR		03ECH			 

#pragma ADDRESS		P7_ADDR		03EDH			 

#pragma ADDRESS		P6D_ADDR	03EEH			 

#pragma ADDRESS		P7D_ADDR	03EFH			 

#pragma ADDRESS		P8_ADDR		03F0H			 

#pragma ADDRESS		P9_ADDR		03F1H			 

#pragma ADDRESS		P8D_ADDR	03F2H			 

#pragma ADDRESS		P9D_ADDR	03F3H			 

#pragma ADDRESS		P10_ADDR	03F4H			 

#pragma ADDRESS		P11_ADDR	03F5H			 

#pragma ADDRESS		P10D_ADDR	03F6H			 

#pragma ADDRESS		P1D1_ADDR	03F7H			 

#pragma ADDRESS		P12_ADDR	03F8H			 

#pragma ADDRESS		P13_ADDR	03F9H			 

#pragma ADDRESS		P1D2_ADDR	03FAH			 

#pragma ADDRESS		P1D3_ADDR	03FBH			 

#pragma ADDRESS		PUR0_ADDR	03FCH			 

#pragma ADDRESS		PUR1_ADDR	03FDH			 

#pragma ADDRESS		PUR2_ADDR	03FEH			 

#pragma ADDRESS		PCR_ADDR	03FFH			 

 


unsigned char	DA0_ADDR;						 


unsigned char	DA1_ADDR;						 


 


unsigned char	UDF_ADDR;


 


 


unsigned short	 TA11_ADDR;						 


unsigned short	 TA21_ADDR;						 


unsigned short	 TA41_ADDR;						 


unsigned short	 TB3_ADDR;						 


unsigned short	 TB4_ADDR;						 


unsigned short	 TB5_ADDR;						 


unsigned short	 TA0_ADDR;						 


unsigned short	 TA1_ADDR;						 


unsigned short	 TA2_ADDR;						 


unsigned short	 TA3_ADDR;						 


unsigned short	 TA4_ADDR;						 


unsigned short	 TB0_ADDR;						 


unsigned short	 TB1_ADDR;						 


unsigned short	 TB2_ADDR;						 


 


struct BIT_DEF {
		char	B0:1;
		char	B1:1;
		char	B2:1;
		char	B3:1;
		char	B4:1;
		char	B5:1;
		char	B6:1;
		char	B7:1;
};
union BYTE_DEF{
	struct BIT_DEF BIT;
	char	BYTE;
};

 


union BYTE_DEF PM0_ADDR;











 


union BYTE_DEF PM1_ADDR;










 


union BYTE_DEF CM0_ADDR;











 


union BYTE_DEF CM1_ADDR;








 


union BYTE_DEF CSR_ADDR;











 


union BYTE_DEF AIER_ADDR;





 


union BYTE_DEF PRCR_ADDR;






 


union BYTE_DEF DBR_ADDR;







 


union BYTE_DEF CM2_ADDR;








 


union BYTE_DEF WDTS_ADDR;


 


union BYTE_DEF WDC_ADDR;




 


union BYTE_DEF CSE_ADDR;











 


union BYTE_DEF PLC0_ADDR;








 


union BYTE_DEF PM2_ADDR;




 


union BYTE_DEF DM0CON_ADDR;









 


union BYTE_DEF	DM1CON_ADDR;









 


union BYTE_DEF INT3IC_ADDR;








 


union BYTE_DEF TB5IC_ADDR;







 


union BYTE_DEF TB4IC_ADDR;







 


union BYTE_DEF TB3IC_ADDR;







 


union BYTE_DEF S4IC_ADDR;








 


union BYTE_DEF S3IC_ADDR;








 


union BYTE_DEF INT5IC_ADDR;








 


union BYTE_DEF INT4IC_ADDR;








 


union BYTE_DEF BCNIC_ADDR;







 


union BYTE_DEF DM0IC_ADDR;







 


union BYTE_DEF DM1IC_ADDR;







 


union BYTE_DEF KUPIC_ADDR;







 


union BYTE_DEF ADIC_ADDR;







 


union BYTE_DEF S2TIC_ADDR;







 


union BYTE_DEF S2RIC_ADDR;







 


union BYTE_DEF S0TIC_ADDR;







 


union BYTE_DEF S0RIC_ADDR;







 


union BYTE_DEF S1TIC_ADDR;







 


union BYTE_DEF S1RIC_ADDR;







 


union BYTE_DEF TA0IC_ADDR;







 


union BYTE_DEF TA1IC_ADDR;







 


union BYTE_DEF TA2IC_ADDR;







 


union BYTE_DEF TA3IC_ADDR;







 


union BYTE_DEF TA4IC_ADDR;







 


union BYTE_DEF TB0IC_ADDR;







 


union BYTE_DEF TB1IC_ADDR;







 


union BYTE_DEF TB2IC_ADDR;







 


union BYTE_DEF INT0IC_ADDR;








 


union BYTE_DEF INT1IC_ADDR;








 


union BYTE_DEF INT2IC_ADDR;








 


union BYTE_DEF FMR1_ADDR;





 


union BYTE_DEF FMR0_ADDR;










 


union BYTE_DEF AIER2_ADDR;





 


union BYTE_DEF PCLKR_ADDR;





 


union BYTE_DEF TBSR_ADDR;






 


union BYTE_DEF INVC0_ADDR;











 


union BYTE_DEF INVC1_ADDR;











 


union BYTE_DEF IDB0_ADDR;









 


union BYTE_DEF IDB1_ADDR;









 


union BYTE_DEF DTT_ADDR;


 


union BYTE_DEF ICTB2_ADDR;


 


union BYTE_DEF TB3MR_ADDR;











 


union BYTE_DEF TB4MR_ADDR;










 


union BYTE_DEF TB5MR_ADDR;










 


union BYTE_DEF IFSR2A_ADDR;





 


union BYTE_DEF IFSR_ADDR;











 


union BYTE_DEF S3TRR_ADDR;


 


union BYTE_DEF S3C_ADDR;











 


union BYTE_DEF S3BRG_ADDR;


 


union BYTE_DEF S4TRR_ADDR;


 


union BYTE_DEF S4C_ADDR;











 


union BYTE_DEF S4BRG_ADDR;


 


union BYTE_DEF U0SMR4_ADDR;











 


union BYTE_DEF U0SMR3_ADDR;








 


union BYTE_DEF U0SMR2_ADDR;










 


union BYTE_DEF U0SMR_ADDR;










 


union BYTE_DEF U1SMR4_ADDR;











 


union BYTE_DEF U1SMR3_ADDR;








 


union BYTE_DEF U1SMR2_ADDR;










 


union BYTE_DEF U1SMR_ADDR;










 


union BYTE_DEF U2SMR4_ADDR;











 


union BYTE_DEF U2SMR3_ADDR;








 


union BYTE_DEF U2SMR2_ADDR;










 


union BYTE_DEF U2SMR_ADDR;










 


union BYTE_DEF U2MR_ADDR;











 


union BYTE_DEF U2BRG_ADDR;


 


union BYTE_DEF U2C0_ADDR;











 


union BYTE_DEF U2C1_ADDR;











 


union BYTE_DEF TABSR_ADDR;











 


union BYTE_DEF CPSRF_ADDR;




 


union BYTE_DEF ONSF_ADDR;











 


union BYTE_DEF TRGSR_ADDR;











 


union BYTE_DEF TA0MR_ADDR;











 


union BYTE_DEF TA1MR_ADDR;











 


union BYTE_DEF TA2MR_ADDR;











 


union BYTE_DEF TA3MR_ADDR;











 


union BYTE_DEF TA4MR_ADDR;











 


union BYTE_DEF TB0MR_ADDR;











 


union BYTE_DEF TB1MR_ADDR;










 


union BYTE_DEF TB2MR_ADDR;










 


union BYTE_DEF TB2SC_ADDR;





 


union BYTE_DEF U0MR_ADDR;











 


union BYTE_DEF U0BRG_ADDR;


 


union BYTE_DEF U0C0_ADDR;











 


union BYTE_DEF U0C1_ADDR;









 


union BYTE_DEF U1MR_ADDR;











 


union BYTE_DEF U1BRG_ADDR;


 


union BYTE_DEF U1C0_ADDR;











 


union BYTE_DEF U1C1_ADDR;









 


union BYTE_DEF UCON_ADDR;










 


union BYTE_DEF FIDR_ADDR;





 


union BYTE_DEF DM0SL_ADDR;









 


union BYTE_DEF DM1SL_ADDR;









 


union BYTE_DEF CRCIN_ADDR;


 


union BYTE_DEF ADCON2_ADDR;







 


union BYTE_DEF ADCON0_ADDR;











 


union BYTE_DEF ADCON1_ADDR;











 


union BYTE_DEF DACON_ADDR;





 


union BYTE_DEF PC14_ADDR;







 


union BYTE_DEF PUR3_ADDR;











 


union BYTE_DEF P0_ADDR;











 


union BYTE_DEF P1_ADDR;











 


union BYTE_DEF P0D_ADDR;











 


union BYTE_DEF P1D_ADDR;











 


union BYTE_DEF P2_ADDR;











 


union BYTE_DEF P3_ADDR;











 


union BYTE_DEF P2D_ADDR;











 


union BYTE_DEF P3D_ADDR;











 


union BYTE_DEF P4_ADDR;











 


union BYTE_DEF P5_ADDR;











 


union BYTE_DEF P4D_ADDR;











 


union BYTE_DEF P5D_ADDR;











 


union BYTE_DEF P6_ADDR;











 


union BYTE_DEF P7_ADDR;











 


union BYTE_DEF P6D_ADDR;











 


union BYTE_DEF P7D_ADDR;











 


union BYTE_DEF P8_ADDR;











 


union BYTE_DEF P9_ADDR;











 


union BYTE_DEF P8D_ADDR;










 


union BYTE_DEF P9D_ADDR;











 


union BYTE_DEF P10_ADDR;











 


union BYTE_DEF P11_ADDR;











 


union BYTE_DEF P10D_ADDR;











 


union BYTE_DEF P1D1_ADDR;











 


union BYTE_DEF P12_ADDR;











 


union BYTE_DEF P13_ADDR;











 


union BYTE_DEF P1D2_ADDR;











 


union BYTE_DEF P1D3_ADDR;











 


union BYTE_DEF PUR0_ADDR;











 


union BYTE_DEF PUR1_ADDR;











 


union BYTE_DEF PUR2_ADDR;









 


union BYTE_DEF PCR_ADDR;




 


union{
	struct{
		char	B0:1;
		char	B1:1;
		char	B2:1;
		char	B3:1;
		char	B4:1;
		char	B5:1;
		char	B6:1;
		char	B7:1;
		char	B8:1;
		char	B9:1;
		char	B10:1;
		char	B11:1;
		char	B12:1;
		char	B13:1;
		char	B14:1;
		char	B15:1;
	}BIT;
	struct{
		char	LOW;							 
		char	HIGH;							 
	}BYTE;
	unsigned short	WORD;
}TCR0_ADDR,TCR1_ADDR,
 U0TB_ADDR,U1TB_ADDR,U2TB_ADDR,U0RB_ADDR,U1RB_ADDR,U2RB_ADDR,
 CRCD_ADDR,
 AD0_ADDR,AD1_ADDR,AD2_ADDR,AD3_ADDR,AD4_ADDR,AD5_ADDR,AD6_ADDR,AD7_ADDR;

 






 






 






 






 






 












 












 













 






 






 






 






 






 






 






 






 






 


union{
	struct{
		char	B0:1;
		char	B1:1;
		char	B2:1;
		char	B3:1;
		char	B4:1;
		char	B5:1;
		char	B6:1;
		char	B7:1;
		char	B8:1;
		char	B9:1;
		char	B10:1;
		char	B11:1;
		char	B12:1;
		char	B13:1;
		char	B14:1;
		char	B15:1;
		char	B16:1;
		char	B17:1;
		char	B18:1;
		char	B19:1;
	}BIT;
	struct{
		char	LOW;							 
		char	MID;							 
		char	HIGH;							 
		char	NC;								 
	}BYTE;
	unsigned long	DWORD;
}RMAD0_ADDR,RMAD1_ADDR,RMAD2_ADDR,RMAD3_ADDR,
 SAR0_ADDR,SAR1_ADDR,
 DAR0_ADDR,DAR1_ADDR;

 







 







 







 







 







 







 







 







void Initialise( void );
#line 2 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/GLCD.C"

#line 1 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/glcd.h"



#line 1 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/font.h"



typedef struct FONT_DEF{
	unsigned char u8Width;  
	unsigned char u8Height;  
	unsigned char *au8FontTable;  
} FONT_DEF;

extern FONT_DEF Font_System5x8;
extern FONT_DEF Font_System7x8;

extern unsigned char FontSystem3x6[195];
extern unsigned char FontSystem5x8[485];
extern unsigned char FontSystem7x8[679];


#line 4 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/glcd.h"

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

 


 








 






 











#line 3 "C:/Users/Miguel/Desktop/Nova pasta/NG2S901_As1_Menu/NG2S901_As1_Menu/drivers/glcd/GLCD.C"

FONT_DEF Font_System3x6  = {3, 6,FontSystem3x6};
FONT_DEF Font_System5x8  = {5, 8, FontSystem5x8};
FONT_DEF Font_System7x8  = {7, 8, FontSystem7x8};

 
void LcdSelectSide(unsigned char);
void LcdDataWrite (unsigned char);
void LcdInstructionWrite (unsigned char);
unsigned char LcdDataRead (void);
void LcdSetDot (unsigned char, unsigned char);
void LcdWaitBusy (void);
void LcdDelay (unsigned long);
 

 
unsigned char u8CursorX, u8CursorY;

 
 
 

 



void LcdDelay(unsigned long u32Duration)
{
	unsigned long u32Delay;
	for (u32Delay=0; u32Delay<u32Duration; u32Delay++);
}

 



void GLCD_Initialise(void)	
{
	P2_ADDR.BYTE     = 0;
	P3_ADDR.BIT.B1		   	 = 0;
  	P3_ADDR.BIT.B3		   	 = 0;
  	P3_ADDR.BIT.B0		   	 = 0;
  	P3_ADDR.BIT.B4		   	 = 0;
  	P3_ADDR.BIT.B5		   	 = 0;
  	
  	P3_ADDR.BIT.B2		   	 = 1;
  	LcdDelay(10);
  	P3_ADDR.BIT.B2		   	 =0;
  	LcdDelay(10);
  	P3_ADDR.BIT.B2		   	 =1;
  	  	
  	LcdSelectSide(1 );
  	LcdInstructionWrite(0x3E	  );  
  	LcdInstructionWrite(0xC0	  );
  	LcdInstructionWrite(0xB8	  );
  	LcdInstructionWrite(0x40	  );
  	LcdInstructionWrite(0x3F	  );  
  	
  	LcdSelectSide(0 );
  	LcdInstructionWrite(0x3E	  );  
  	LcdInstructionWrite(0xC0	  );
  	LcdInstructionWrite(0xB8	  );
  	LcdInstructionWrite(0x40	  );
  	LcdInstructionWrite(0x3F	  );  
  	
  	GLCD_ClearScreen();
}  	

 




void LcdSelectSide(unsigned char u8LcdSide)
{
	if(u8LcdSide == 0 )
	{
		 
		P3_ADDR.BIT.B0		   	 =0;
		P3_ADDR.BIT.B1		   	 =0;
              	P3_ADDR.BIT.B3		   	 =1;
              	P3_ADDR.BIT.B4		   	 =0; 	
              	P3_ADDR.BIT.B5		   	 =1;
	
              	LcdInstructionWrite(0x40	  );  
        }
	else
	{
		 
		P3_ADDR.BIT.B0		   	 =0;
		P3_ADDR.BIT.B1		   	 =0;
              	P3_ADDR.BIT.B3		   	 =1;
              	P3_ADDR.BIT.B4		   	 =1; 	
              	P3_ADDR.BIT.B5		   	 =0;
	           
              	LcdInstructionWrite(0x40	  );  
	}
}

 




void LcdDataWrite (unsigned char u8Data)
{
	LcdWaitBusy ();		 
	P3_ADDR.BIT.B1		   	 = 1;      	 
	P3_ADDR.BIT.B3		   	 = 0;      	 
	
	P2_ADDR.BYTE     = u8Data;	 
	
	P3_ADDR.BIT.B0		   	 = 1;		 
	LcdDelay(1);	
	P3_ADDR.BIT.B0		   	 = 0;
}

 




void LcdInstructionWrite (unsigned char u8Instruction)
{
   	LcdWaitBusy ();		 
   	P3_ADDR.BIT.B1		   	 = 0;       	 
  	P3_ADDR.BIT.B3		   	 = 0;       	 
	 	
  	P2_ADDR.BYTE     = u8Instruction; 
  	  	
  	P3_ADDR.BIT.B0		   	 = 1;		 
  	LcdDelay(1);
  	P3_ADDR.BIT.B0		   	 = 0;
}

 



unsigned char LcdDataRead ()
{
	P2_ADDR.BYTE     = 0xFF;	 
	
  	P3_ADDR.BIT.B3		   	 = 1;       	 
  	P3_ADDR.BIT.B1		   	 = 1;       	 
  	
  	P3_ADDR.BIT.B0		   	  = 0;       	 
  	LcdDelay(1);
	P3_ADDR.BIT.B0		   	  = 1;
	LcdDelay(1);
	  	  	
	return P2_ADDR.BYTE     ;	 
}

 



void LcdWaitBusy ()
{
	P2_ADDR.BYTE     =0xFF;		 
	  	
  	P3_ADDR.BIT.B1		   	 = 0;       	 
  	P3_ADDR.BIT.B3		   	 = 1;       	 
  	
  	P3_ADDR.BIT.B0		   	  = 0;       	 
  	LcdDelay(1);
  	P3_ADDR.BIT.B0		   	  = 1;
	
	while (P2_ADDR.BYTE     & 0x7F == 0x80 );  

}

 



void GLCD_ClearScreen(void)
{
  	 unsigned char u8Page=0;
  	 unsigned char u8Column=0;
    
    	 
  	for (u8Page = 0; u8Page < 8; u8Page++) 
    	{
      		LcdSelectSide(1 );			 
      		LcdInstructionWrite(0xB8	  | u8Page);  
      		LcdInstructionWrite(0x40	  );		 
      		
      		 
      		for (u8Column = 0; u8Column < 128; u8Column++) 
		{
	  		if (u8Column == 64)
            			{    
              			LcdSelectSide(0 );	 
              			LcdInstructionWrite(0xB8	  | u8Page);  
              			LcdInstructionWrite(0x40	  );  
      		             	}
          		LcdDataWrite (0x00);		 
		}
    	}
}

 




void GLCD_DisplayPicture (unsigned char *au8PictureData)
{
	 unsigned char u8Page=0;
  	 unsigned char u8Column=0;
      
         
        LcdSelectSide(1 );
  	for (u8Page = 0; u8Page < 8; u8Page++)  
    	{
      		LcdInstructionWrite(0xB8	  | u8Page);  
      		for (u8Column = 0; u8Column < 64; u8Column++)
			LcdDataWrite(au8PictureData[(128*u8Page)+u8Column]);
	}
	
	 
	LcdSelectSide(0 );
  	for (u8Page = 0; u8Page < 8; u8Page++)  
    	{
      		LcdInstructionWrite(0xB8	  | u8Page);  
      		for (u8Column = 64; u8Column < 128; u8Column++)
			LcdDataWrite(au8PictureData[(128*u8Page)+u8Column]);
	}	
}

 	





void LcdSetDot (unsigned char u8Xaxis, unsigned char u8Yaxis)
{
 	 unsigned char u8DataRead=0;
 	
 	LcdInstructionWrite(0xC0	  );  

	 
	if (u8Xaxis < 64)
	{
    		LcdSelectSide (1 );				 
    		LcdInstructionWrite (0xB8	  + (u8Yaxis / 8));  
    		LcdInstructionWrite (0x40	  + u8Xaxis);	 
    		
    		u8DataRead = LcdDataRead ();  			 
    		u8DataRead = LcdDataRead ();  			 
    		
    		LcdInstructionWrite (0xB8	  + (u8Yaxis / 8));  
    		LcdInstructionWrite (0x40	  + u8Xaxis);	 
    		LcdDataWrite (u8DataRead | (1 << (u8Yaxis % 8))); 
    	} 
  	else 
  	 
  	{
    		LcdSelectSide(0 );				          
    		LcdInstructionWrite (0xB8	  + (u8Yaxis / 8));         
    		LcdInstructionWrite (0x40	  + u8Xaxis - 64);               
    		                                                                               
    		u8DataRead = LcdDataRead ();                     
    		u8DataRead = LcdDataRead ();                        
    		                                                                               
    		LcdInstructionWrite (0xB8	  + (u8Yaxis / 8));         
    		LcdInstructionWrite (0x40	  + u8Xaxis - 64);               
    		LcdDataWrite (u8DataRead | (1 << (u8Yaxis % 8)));    
    		
    	}
      	LcdInstructionWrite(0xC0	  );  
}

 




void GLCD_WriteChar (unsigned char u8Char, FONT_DEF toto)
{
	unsigned char u8CharColumn=0,u8RightSide=0;
	unsigned char u8UpperCharPointer=1,u8Page=0;
	
	 
	
	 
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
	
	 
	if (u8CursorX > 128 - (toto.u8Width) ) 
	{	
		u8CursorX = 0;
		u8CursorY++;
		if (u8CursorY == 8) u8CursorY = 0;
	}
	
	 
	if (u8CursorX < 64)
	{	
		 
		LcdSelectSide (1 );
		LcdInstructionWrite (0x40	  + u8CursorX);
	}
	else 
	{
		 
		LcdSelectSide (0 );
		LcdInstructionWrite (0x40	  + u8CursorX - 64);
	}
	    	
     
	while (u8CharColumn < (toto.u8Width) )
	{
		if ((toto.u8Height) > 8)
		{
			u8UpperCharPointer = 2;
			if (u8CursorX > 64)
				u8RightSide=64;
			LcdInstructionWrite (0xB8	  + u8CursorY -1); 
			LcdInstructionWrite (0x40	  + u8CursorX -u8RightSide);
			LcdDataWrite ((toto.au8FontTable)[( (u8Char - 32) * u8UpperCharPointer * (toto.u8Width) ) + ((u8CharColumn * u8UpperCharPointer) + 1)]);
			LcdInstructionWrite (0x40	  + u8CursorX -u8RightSide);
			
		}
		LcdInstructionWrite (0xB8	  + u8CursorY); 
		LcdDataWrite ((toto.au8FontTable)[( (u8Char - 32) * u8UpperCharPointer * (toto.u8Width) ) + (u8CharColumn * u8UpperCharPointer)]);		
		u8CharColumn++;
		u8CursorX++;
		
		 
		if (u8CursorX == 64)
		{
			u8RightSide=64;
			LcdSelectSide (0 );	 
			LcdInstructionWrite (0xB8	  + u8CursorY);
			LcdInstructionWrite (0x40	  + u8CursorX -u8RightSide);
		}
	}
	
	 	
	if (u8CursorX < 128) 		 
	{
		if ((toto.u8Height) > 8)
		{
			LcdInstructionWrite (0xB8	  + u8CursorY - 1);  
			LcdInstructionWrite (0x40	  + u8CursorX);
			LcdDataWrite(0x00);
			LcdInstructionWrite (0x40	  + u8CursorX -u8RightSide);
		}
		LcdInstructionWrite (0xB8	  + u8CursorY);  
		LcdInstructionWrite (0x40	  + u8CursorX );	
		LcdDataWrite(0x00); 	 
	}
			
	u8CharColumn++;
	u8CursorX++;
}

 




void GLCD_WriteString (unsigned char *au8Text, FONT_DEF toto) 
{
	while(*au8Text != 0)
	{
		GLCD_WriteChar (*au8Text,toto);
		au8Text++;
	}
}

 





Point GLCD_GoTo(unsigned char u8Column, unsigned char u8Line)
{
	Point oldPoint;
	oldPoint.x = u8CursorX;
	oldPoint.y = u8CursorY;
	u8CursorX = u8Column;
	u8CursorY = u8Line;
	return oldPoint;
}

 








void GLCD_DisplayValue (unsigned long Number,  char Digits, FONT_DEF toto) 
{
	 char Size_of_String=0; 	 
	 unsigned char Number_String[10];	 
	 char Position=0;					 
	 char tt=0;
		Size_of_String=Digits;
		Number_String[Size_of_String] = 0;
		Size_of_String=Size_of_String-1;
		for(Position=0 ;Position<=Size_of_String;Position++)
		{
		
			Number_String[Size_of_String-Position] = Number % 10 + '0';
			Number /= 10;
		}
	 
	GLCD_WriteString(Number_String,toto);
}	

 







void GLCD_Rectangle (unsigned char u8Xaxis1,unsigned char u8Yaxis1,unsigned char u8Xaxis2,unsigned char u8Yaxis2)
{
  	 unsigned char u8CurrentValue=0;

	 
  	for (u8CurrentValue = 0; u8CurrentValue < u8Xaxis2 - u8Xaxis1+ 1; u8CurrentValue++) 
  	{
		LcdSetDot(u8Xaxis1 + u8CurrentValue, u8Yaxis1);
		LcdSetDot(u8Xaxis1 + u8CurrentValue, u8Yaxis2);
	}
  	
  	 
  	for (u8CurrentValue = 0; u8CurrentValue < u8Yaxis2 - u8Yaxis1 + 1; u8CurrentValue++)	
  	{
		LcdSetDot(u8Xaxis1, u8Yaxis1 + u8CurrentValue);
		LcdSetDot(u8Xaxis2, u8Yaxis1 + u8CurrentValue);
	}
}

 






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
