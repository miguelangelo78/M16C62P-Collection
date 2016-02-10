#ifndef ISR_DEFS_H_
#define ISR_DEFS_H_

enum INTERRUPT_VECTOR {
	INT0_T, INT1_T, ADC0_T, ADC1_T, ADC2_T, INTMAX
};

#define ADC_CHANNEL_COUNT 3

typedef char (*isr_cback_t)(void);

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
void init_adc(char channel); /* Initializes ADC with more options */
void init_adc_basic(char channel); /* Initializes ADC with less options. It might be useful for fast configuration */
void deinit_adc(char channel);
void adc_start(void); /* Triggers ADC conversion */
void adc_stop(void);
void deinit_all_adc(void);
void install_cback(isr_cback_t callback, char cback_index);
void uninstall_cback(char cback_index);
void uninstall_all_cback(void);

#endif
