#ifndef CPU_FREQ_
#define CPU_FREQ_

inline void cpu_init() {
	int count = 10000;
	prc0 = 1;
	cm1 = 0x20;
	cm06 = 0;
	prc1 = 1;
	pm20 = 1;
	prc1 = 0;
	plc0 = 0x12;
	plc07 = 1;
	while(count--);
	cm11 = 1;
	prc0 = 0;
	prc0 = 1;
	pd8_7 = 0;
	pd8_6 = 0;
	cm04 = 1;
	prc1 = 1;
	pm10 = 1;
	prc1 = 0;	
}

#endif