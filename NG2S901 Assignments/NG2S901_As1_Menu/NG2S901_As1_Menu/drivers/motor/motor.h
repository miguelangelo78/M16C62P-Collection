#ifndef MOTOR_H_
#define MOTOR_H_

#define MAX_MOTOR_PITCH 500

void init_motor_sound(void);
void deinit_motor_sound(void);
void motor_tone_us(int pitch, int duration_us);
void motor_tone(int pitch, int duration_ms);

#endif