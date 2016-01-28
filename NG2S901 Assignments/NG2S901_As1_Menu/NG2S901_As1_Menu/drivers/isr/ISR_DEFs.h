#pragma interrupt Timer_A0
	void Timer_A0(void);

#pragma interrupt Timer_A1
	void Timer_A1(void);

#pragma interrupt Timer_A2
	void Timer_A2(void);

#pragma interrupt Timer_A3
	void Timer_A3(void);
	
#pragma interrupt Timer_A4
	void Timer_A4(void);

#pragma interrupt ADC_Interrupt
	void ADC_Interrupt(void);

#pragma interrupt INT0
	void INT0(void);

#pragma interrupt INT1
	void INT1(void);

#pragma interrupt U0rec_ISR
void U0rec_ISR(void);

void init_isr(void);
void install_cback(isr_cback_t callback, char cback_index);
void uninstall_cback(char cback_index);
void uninstall_all_cback(void);