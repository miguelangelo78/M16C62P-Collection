#ifndef LED_H_
#define LED_H_

#define DIM_DELAY_MAX 100
#define DIM_BRIGHTNESS_MAX (DIM_DELAY_MAX / 2)

/* Pattern types available: */
enum LED_PATT_TYPE {
	LED_PATT_SHIFTSIDES,
	LED_PATT_DIM_ALL,
	LED_PATT_COUNTUP
};

void led_pattern_update(char patt_type);

/* Variables to control the LED pattern like the speed and brightness: */

extern unsigned int countup_delay_max;

extern unsigned int dim_delay;
extern unsigned int dim_brightness;

#endif