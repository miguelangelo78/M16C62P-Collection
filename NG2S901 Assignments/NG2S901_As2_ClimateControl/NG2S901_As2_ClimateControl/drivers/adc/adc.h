#ifndef ADC_H_
#define ADC_H_

#define ADC_CHANNEL_COUNT 3

typedef void (*adc_cback_t)(void);

enum ADC_MODE {
	ADC_MODE_ONESHOT,
	ADC_MODE_REPEAT,
	ADC_MODE_SINGLESWEEP,
	ADC_MODE_REPEATSWEEP
};

enum ADC_PORTSEL {
	ADC_PORTSEL_P10, ADC_PORTSEL_DISABLED, ADC_PORTSEL_P0, ADC_PORTSEL_P2	
};

#define ADC_DEFAULT -1
#define ADC_RESTART() ADST = 1

void adc_init_easy(adc_cback_t adc_callback, char channels);
void adc_init(adc_cback_t adc_callback, char channel, char adc_mode, char tenbits_size, char sampleandhold, int portsel);
void adc_deinit(void);
unsigned int adc_read(char channel);

#endif