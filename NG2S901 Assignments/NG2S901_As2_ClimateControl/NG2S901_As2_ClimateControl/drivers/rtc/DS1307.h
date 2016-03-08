#ifndef DS1307_H_
#define DS1307_H_

/* RTC (DS1307) Datasheet: http://datasheets.maximintegrated.com/en/ds/DS1307.pdf */

typedef struct {
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t weekday;
	uint8_t monthday;
	uint8_t month;
	uint16_t year;
	uint8_t sqw;
} rtc_t;

enum RTC_TIME_MODE {
	RTC_AM, RTC_PM	
};

void init_rtc(void);
void deinit_rtc(void);

rtc_t * read_clock(void);

uint8_t rtc_get_seconds(void);
uint8_t rtc_get_minutes(void);
uint8_t rtc_get_hours(void);
uint8_t rtc_get_weekday(void);
uint8_t rtc_get_monthday(void);
uint8_t rtc_get_month(void);
uint16_t rtc_get_year(void);
void rtc_switch_mode(char time_mode);

void rtc_set_clock(rtc_t * new_time);

void rtc_set_seconds(uint8_t);
void rtc_set_minutes(uint8_t);
void rtc_set_hours(uint8_t);
void rtc_set_weekday(uint8_t);
void rtc_set_monthday(uint8_t);
void rtc_set_month(uint8_t);
void rtc_set_year(uint16_t);

void rtc_stop();
char rtc_is_stopped(void);
void rtc_start();
void rtc_reset(void);
uint8_t rtc_has_ticked(void);

char * rtc_read_time_formatted(uint8_t get_days);

#endif