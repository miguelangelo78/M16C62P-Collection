#ifndef CPUFREQ_H_
#define CPUFREQ_H_

#define CPU_FREQ 24000000 /* 24MHz */

inline void cpu_init(void) {
	unsigned short count = 10000;

	/* configure main clock */

	/* configure clock for divide by 1 mode */
	/* enable access to clock registers */
    prc0 = 1;

	/* set CM16,CM17 divide ratio to 1, main clock on in high drive no PLL*/
    cm1 = 0x20;

	/* set divide ratio to 1 */
	cm06 = 0;							
	
	/** 
	* configure and switch main clock to PLL at 24MHz - comment this section
	* out if PLL operation is not desired 
	**/

    /* allow writing to processor mode register */
	prc1 = 1;					

	/* set SFR access to 2 wait which is required for operation greater than 16 MHz */
	pm20 = 1;					

	/* protect processor mode register */
	prc1 = 0;					

	/* enable PLL in X4 mode then turn PLL on */
    plc0 = 0x12;

	/* enable PLL */
	plc07 = 1;					

	/* wait for PLL to stabilize (20mS maximum, 10,000 cycles @6Mhz) this decrement with no optimization 
	   is 12 cycles each, after delay switch clock to PLL */
    while(count--);
	
	/* switch to PLL */
    cm11 = 1;

	/* if not using PLL comment out to this line */

    /* protect clock control register */
	prc0 = 0;   				

	/* configure 32 kHz clock if not using 32 kHz clock comment this section out */

   	/* Unlock CM0 and CM1 */
	prc0 = 1;

	/* set XCin/XCout port pins to inputs*/
   	pd8_7 = 0;		
   	pd8_6 = 0;

 	/* Start the 32KHz crystal a delay must occur before a device uses this clock */
   	cm04 = 1; 		
}

#endif