#include "../../globals.h"
#include <string.h>

#define RTC_I2C_ADDRESS 0x68

static rtc_t time;
static rtc_t time_bcd_copy;

/* RTC Table offsets: */
enum RTC_TABLE {
	TSECS, TMINS, THOURS, TWDAY, TMDAY, TMONTH, TYEAR, TSQW	
};

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
	time_bcd_copy.hour = bcd2dec(bcd_rtc->hour & 0x3F);
	time_bcd_copy.weekday = bcd2dec(bcd_rtc->weekday);
	time_bcd_copy.monthday = bcd2dec(bcd_rtc->monthday);
	time_bcd_copy.month = bcd2dec(bcd_rtc->month);
	time_bcd_copy.year = bcd2dec(bcd_rtc->year);
	return &time_bcd_copy;
}

inline void rtc_write(unsigned char reg, unsigned char val) {
	char data[2];
	data[0]=reg;
	data[1]=val;
	i2c_send_str(RTC_I2C_ADDRESS, data, 2);
}

inline void rtc_selreg(unsigned char reg) {
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
	update_clock();
}

void deinit_rtc(void) {
	i2c_deinit();	
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
	return (uint16_t)(bcd2dec(time.year) * 2066) / 99;	
}

/* SETTERS: */
void rtc_set_clock(rtc_t * new_time) {
	if(new_time->sec < 60 && new_time->sec >= 0)
		time.sec = dec2bcd(new_time->sec) | time.sec & 0x80;
	if(new_time->min < 60 && new_time->min >= 0)
		time.min = dec2bcd(new_time->min);
	if(new_time->hour <= 24 && new_time->hour >= 0)
		time.hour = dec2bcd(new_time->hour) | time.hour & 0x40;
	if(new_time->weekday > 0 && new_time->weekday < 8)
		time.weekday = dec2bcd(new_time->weekday);
	if(new_time->monthday > 0 && new_time->monthday < 32)
		time.monthday = dec2bcd(new_time->monthday);
	if(new_time->month > 0 && new_time->weekday < 13)
		time.month = dec2bcd(new_time->month);
	if(new_time->year > 0 && new_time->year < 100)
		time.year = dec2bcd((uint8_t)(new_time->year * 99) / 2066);
	
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
	if(hours <= 24 && hours >= 0)
		time.hour = dec2bcd(hours) | time.hour & 0x40;
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
	year = (uint16_t)(year * 99) / 2066;
	if(year > 0 && year < 100)
		time.year = dec2bcd(year);
	update_new_clock();
}

/* CONTROL CLOCK: */

void stop() {
	time.sec |= 0x80;
	rtc_write(TSECS, time.sec);
}

char is_stopped(void) {
	return (time.sec & 0x80 == 0x80);	
}

void start() {
	time.sec &= ~0x80;
	rtc_write(TSECS, time.sec);	
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
				"%.2d:%.2d:%.2d\n%.2d/%.2d/%.4d Week %.2d", 
				tptr->hour, tptr->min, tptr->sec, tptr->monthday, tptr->month, tptr->year, tptr->weekday);
	else
		sprintf(time_formatted_buffer, 
				"%.2d:%.2d:%.2d", 
				tptr->hour, tptr->min, tptr->sec);
	return time_formatted_buffer;
}