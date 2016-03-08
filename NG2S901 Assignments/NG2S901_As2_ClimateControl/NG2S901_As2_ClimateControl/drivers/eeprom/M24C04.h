#ifndef M24C04_H_
#define M24C04_H_

void eeprom_init(void);
void eeprom_deinit(void);
void eeprom_write(uint8_t address, uint8_t value);
void eeprom_write_buff(uint8_t start_address, uint8_t length, uint8_t * buffptr);
uint8_t eeprom_read(uint8_t address);
uint8_t * eeprom_read_buff(uint8_t start_address, uint8_t length);

#endif