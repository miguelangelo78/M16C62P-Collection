#ifndef SEVENSEG_H_
#define SEVENSEG_H_

void init_7seg();
void seg_update(unsigned char n, unsigned char radix);
void seg_turnoff(void);

#endif