#include "../../globals.h"

/* M24C04 Datasheet: http://www.st.com/web/en/resource/technical/document/datasheet/DM00071277.pdf */

#define EE_WC P7_3
#define EEPROM_RTC_ADDRESS 0x50

void eeprom_enable_write(void) {
	EE_WC = 0;
}

void eeprom_disable_write(void) {
	EE_WC = 1;
}

void eeprom_init(void) {
	/* (Related to EE_WC) From Datasheet: "This input signal is useful for protecting the entire contents of the memory from inadvertent
	write operations. Write operations are disabled to the entire memory array when Write Control (WC) is driven high. Write operations are enabled when Write Control (WC) is either
	driven low or left floating" */
	P7D_3 = 1; 
	i2c_init();
}

void eeprom_deinit(void) {
	eeprom_disable_write();
}

void eeprom_write(uint8_t address, uint8_t value) {
	char i2c_send_data[2];
	i2c_send_data[0]=address;
	i2c_send_data[1]=value;
	eeprom_enable_write(); /* Unlock write bit */
	i2c_send_str(EEPROM_RTC_ADDRESS, i2c_send_data, 2);
	eeprom_disable_write(); /* Lock write bit */
}

uint8_t eeprom_read(uint8_t address) {
	return i2c_send_and_read(EEPROM_RTC_ADDRESS, address);
}
