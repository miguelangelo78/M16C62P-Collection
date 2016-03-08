#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_MAX_PITCH 500
#define MOTOR_MAX_RPM 15000
#define MOTOR_MIN_RPM 3000

enum MOTOR_COMMAND {
	MOTOR_LEFT, MOTOR_RIGHT, MOTOR_STOP
};

void init_motor(void);
void deinit_motor(void);
void init_motor_sound(void);
void deinit_motor_sound(void);
void motor_stop(void);
void motor_tone_us(int pitch, int duration_us);
void motor_tone(int pitch, int duration_ms);
char motor_get_direction(void);

void motor_spin(unsigned int rpm, char direction);
unsigned int motor_read_rpm(void);

#endif