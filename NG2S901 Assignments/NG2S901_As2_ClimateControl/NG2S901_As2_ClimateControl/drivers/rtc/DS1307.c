#include "../../globals.h"
#include <string.h>

#define RTC_I2C_ADDRESS 0x68

char * weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/* Year takes a special treatment. We can only store up to year 100 on RAM. Therefore, we need to convert,
for example year 2016 to a value between 0 - 100 and backwards from 0 - 100 to 2016 */
#define YEAR_RANGE 100
#define CURRENT_YEAR 2016
#define YEAR_TO_RTCYEAR(year) (map(year, CURRENT_YEAR-YEAR_RANGE/2, CURRENT_YEAR+YEAR_RANGE/2, 0, YEAR_RANGE))
#define RTCYEAR_TO_YEAR(rtcyear) (map(rtcyear, 0, YEAR_RANGE, CURRENT_YEAR-YEAR_RANGE/2, CURRENT_YEAR+YEAR_RANGE/2))

static rtc_t time;
static rtc_t time_bcd_copy;

/* RTC Table offsets: */
enum RTC_TABLE {
	TSECS, TMINS, THOURS, TWDAY, TMDAY, TMONTH, TYEAR, TSQW	
};

char rtc_is12H(void); /* Function prototype */

uint8_t dec2bcd(uint8_t byte) { 
	if(byte == 0) return 0;
	return (((byte / 10) << 4) + (byte % 10));
}

uint8_t bcd2dec(uint8_t byte) {
	if(byte == 0) return 0;
	return (((byte >> 4) * 10) + (byte % 16));	
}

rtc_t * rtc_bcd_to_dec(rtc_t * bcd_rtc) {
	time_bcd_copy.sec = bcd2dec(bcd_rtc->sec & 0x7F);
	time_bcd_copy.min = bcd2dec(bcd_rtc->min);
	if(rtc_is12H())
		time_bcd_copy.hour = bcd2dec(bcd_rtc->hour & 0x1F);
	else
		time_bcd_copy.hour = bcd2dec(bcd_rtc->hour & 0x1F);
	time_bcd_copy.weekday = bcd2dec(bcd_rtc->weekday);
	time_bcd_copy.monthday = bcd2dec(bcd_rtc->monthday);
	time_bcd_copy.month = bcd2dec(bcd_rtc->month);
	time_bcd_copy.year = RTCYEAR_TO_YEAR(bcd2dec(bcd_rtc->year));
	return &time_bcd_copy;
}

void rtc_write(unsigned char reg, unsigned char val) {
	unsigned char data[2];
	data[0]=reg;
	data[1]=val;
	i2c_send_str(RTC_I2C_ADDRESS, data, 2);
}

void rtc_selreg(unsigned char reg) {
	i2c_send(RTC_I2C_ADDRESS, reg);
}

/* Read from RTC: */
void update_clock(void) {
	unsigned char * ptr;
	rtc_selreg(0);
	ptr =  i2c_read_buff(RTC_I2C_ADDRESS, 8);
	time.sec = ptr[TSECS];
	time.min = ptr[TMINS];
	time.hour = ptr[THOURS];
	time.weekday = ptr[TWDAY];
	time.monthday = ptr[TMDAY];
	time.month = ptr[TMONTH];
	time.year = ptr[TYEAR];
	time.sqw = ptr[TSQW];
}

/* Write to RTC: */
void update_new_clock(void) {
	rtc_write(TSECS, time.sec);	
	rtc_write(TMINS, time.min);	
	rtc_write(THOURS, time.hour);	
	rtc_write(TWDAY, time.weekday);	
	rtc_write(TMDAY, time.monthday);	
	rtc_write(TMONTH, time.month);	
	rtc_write(TYEAR, time.year);	
	rtc_write(TSQW, time.sqw);	
}

void init_rtc(void) {
	i2c_init();
	if(rtc_is_stopped())
		rtc_start();
	update_clock();
}

void deinit_rtc(void) {
	/* Might not need to uninitialize stuff for the RTC */	
}

/* Time mode functions (AM/PM):  */
char rtc_is12H(void) {
	return (time.hour & 0x40) == 0x40;
}

char rtc_isPM(void) {
	if(rtc_is12H())
		return (time.hour & 0x20) == 0x20;
	else
		return rtc_get_hours() > 11;
}

void rtc_set24H(void) {
	time.hour = time.hour & ~0x40;
}

void rtc_setAM(void) {
	time.hour = time.hour & ~0x20 | 0x40;
}

void rtc_setPM(void) {
	time.hour = time.hour | 0x60;
}

void rtc_switch_mode(char time_mode) {
	int hour;
	switch(time_mode) {
	case RTC_AM:
		if(rtc_is12H()) return;
		
		if((hour = rtc_get_hours()) < 12) { 
			rtc_setAM();
		} else {
			hour -= 12;
			rtc_setPM();	
		}
		
		if(!hour) hour = 12;
		rtc_set_hours(hour);
		break;
	case RTC_PM: 
		if(!rtc_is12H()) return;
		
		if((hour = rtc_get_hours()) == 12)
			hour = 0;
			
		if(rtc_isPM()) hour += 12;
		
		rtc_set24H();
		rtc_set_hours(hour);
		break;	
	}
}


/* GETTERS: */

rtc_t * read_clock(void) {
	update_clock();
	return rtc_bcd_to_dec(&time);
}

uint8_t rtc_get_seconds(void) {
	update_clock();
	return bcd2dec(time.sec & 0x7F);
}

uint8_t rtc_get_minutes(void) {
	update_clock();
	return bcd2dec(time.min);	
}

uint8_t rtc_get_hours(void) {
	update_clock();
	if(rtc_is12H())
		return bcd2dec(time.hour & 0x1F);
	else
		return bcd2dec(time.hour & 0x3F);	
}

uint8_t rtc_get_weekday(void) {
	update_clock();
	return bcd2dec(time.weekday);	
}

uint8_t rtc_get_monthday(void) {
	update_clock();
	return bcd2dec(time.monthday);	
}

uint8_t rtc_get_month(void) {
	update_clock();
	return bcd2dec(time.month);	
}

uint16_t rtc_get_year(void) {
	update_clock();
	return RTCYEAR_TO_YEAR(bcd2dec(time.year));
}

/* SETTERS: */
void rtc_set_clock(rtc_t * new_time) {
	if(new_time->sec < 60 && new_time->sec >= 0)
		time.sec = dec2bcd(new_time->sec) | time.sec & 0x80;
	if(new_time->min < 60 && new_time->min >= 0)
		time.min = dec2bcd(new_time->min);
	if(rtc_is12H())
		time.hour = dec2bcd(new_time->hour) | time.hour & 0x60;
	else
		time.hour = dec2bcd(new_time->hour) | time.hour & 0x40;
	if(new_time->weekday > 0 && new_time->weekday < 8)
		time.weekday = dec2bcd(new_time->weekday);
	if(new_time->monthday > 0 && new_time->monthday < 32)
		time.monthday = dec2bcd(new_time->monthday);
	if(new_time->month > 0 && new_time->weekday < 13)
		time.month = dec2bcd(new_time->month);
	if(new_time->year > 0 && new_time->year < 100)
		time.year = dec2bcd(YEAR_TO_RTCYEAR(new_time->year));
	
	update_new_clock();
}

void rtc_set_seconds(uint8_t secs) {
	if(secs < 60 && secs >= 0)
		time.sec = dec2bcd(secs) | time.sec & 0x80;
	update_new_clock();
}

void rtc_set_minutes(uint8_t mins) {
	if(mins < 60 && mins >= 0)
		time.min = dec2bcd(mins);
	update_new_clock();	
}

void rtc_set_hours(uint8_t hours) {
	if(rtc_is12H())
		if(hours >= 1 && hours<= 12)
			time.hour = dec2bcd(hours) | time.hour & 0x60;
	else
		if(hours >= 0 && hours <= 24)
			time.hour = dec2bcd(hours) | time.hour & 0x40;	
	update_new_clock();	
}

void rtc_set_weekday(uint8_t weekday) {
	if(weekday > 0 && weekday < 8)
		time.weekday = dec2bcd(weekday);
	update_new_clock();
}

void rtc_set_monthday(uint8_t monthday) {
	if(monthday > 0 && monthday < 32)
		time.monthday = dec2bcd(monthday);
	update_new_clock();	
}

void rtc_set_month(uint8_t month) {
	if(month > 0 && month < 13)
		time.month = dec2bcd(month);
	update_new_clock();	
}

void rtc_set_year(uint16_t year) {
	/* MAX YEAR: 2016 + 50 = 2066
	   MIN YEAR: 2016 - 50 = 1966 */
	year = YEAR_TO_RTCYEAR(year);
	if(year > 0 && year < 100)
		time.year = dec2bcd(year);
	update_new_clock();
}

/* CONTROL CLOCK: */

void rtc_stop() {
	time.sec |= 0x80;
	rtc_write(TSECS, time.sec);
}

char rtc_is_stopped(void) {
	char reg0;
	rtc_selreg(TSECS);
	reg0 = i2c_read(RTC_I2C_ADDRESS);
	return reg0 & 0x80;	
}

void rtc_start() {
	char reg0;
	time.sec &= ~0x80;
	
	rtc_selreg(TSECS);
	reg0 = i2c_read(RTC_I2C_ADDRESS);
	rtc_write(TSECS,reg0 & 0x7F);
}

void rtc_reset(void) {
	char i;
	for(i = TSECS;i < TSQW; i++)
		rtc_write(i, 0);
}

uint8_t old_tick = 0;

uint8_t rtc_has_ticked(void) {
	if(old_tick != time.sec) {
		old_tick = time.sec;
		return 1;
	}
	return 0;
}

/* PRETTY OUTPUT: */

#define TIME_FORMATTED_BUFFER_SIZE 64

char * rtc_read_time_formatted(uint8_t get_days) {
	rtc_t * tptr;
	char time_formatted_buffer[TIME_FORMATTED_BUFFER_SIZE];

	memset(time_formatted_buffer, 0, TIME_FORMATTED_BUFFER_SIZE);
	update_clock();
	tptr = rtc_bcd_to_dec(&time); /* Convert time from BCD to DEC */
	
	if(get_days)
		sprintf(time_formatted_buffer, 
				"%.2d:%.2d:%.2d\n%.2d/%.2d/%.4d %s", 
				tptr->hour, tptr->min, tptr->sec, tptr->monthday, tptr->month, tptr->year, weekdays[tptr->weekday - 1]);
	else
		sprintf(time_formatted_buffer, 
				"%.2d:%.2d:%.2d", 
				tptr->hour, tptr->min, tptr->sec);
	return time_formatted_buffer;
}