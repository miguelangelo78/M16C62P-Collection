#include "globals.h"

extern void LcdSetDot (unsigned char, unsigned char);
extern void GLCD_Rectangle (unsigned char u8Xaxis1,unsigned char u8Yaxis1,unsigned char u8Xaxis2,unsigned char u8Yaxis2);

volatile char enable_extra = 0;

#define BLOCK_COUNT 16

typedef struct {
	int x, y, width, height;
} entity_t;

entity_t player, ball;
entity_t blocks[BLOCK_COUNT];

#define running enable_extra
volatile unsigned int potValue = 0;

void render(void) {
	LcdSetDot(5,5);
}

void update(void) {
	
}

void handle_input(void) {
	/* Exit game: */
	if(getcommand()) enable_extra = 0;
}

void init_entities(void) {
	int i;
	/* Initialize player: */
	player.width = 100;
	player.height= 10;
	player.x = GLCD_WIDTH / 2 - player.width / 2;
	player.y = GLCD_HEIGHT - 1;
	
	/* Initialize ball: */
	ball.width = 2;
	ball.height = ball.width;
	ball.x = player.x + player.width/2 - ball.width / 2;
	ball.y = player.y - ball.height - 5;
	
	/* Initialize blocks: */
	for(i = 0;i < BLOCK_COUNT; i++) {
		blocks[i].width = 50;
		blocks[i].height = 20;
		blocks[i].x = i*blocks[i].width + (i>0 && i<GLCD_WIDTH - 1 ?2:0);
		blocks[i].y = ((i % 2) * i) * blocks[i].height + 2;
	}
}

void main_loop(void) {
	init_entities();
	while(running) {
		render();
		update();
		handle_input();
	}
}

void potentiometer_cback(void) {
	potValue = adc_read(5);
	ADC_RESTART();
}

void extra_deinit(void) {
	init_motor();
	adc_deinit(); /* Uninitialize potentiometer */
	thermistor_init(); /* Reinitialize thermistor */
}

void extra_init(void) {
	deinit_motor();
	thermistor_deinit();
	
	init_motor_sound();
	adc_init(potentiometer_cback, 5, ADC_MODE_ONESHOT, 1, 0, ADC_DEFAULT);
	
	GLCD_ClearScreen();
	GLCD_GoTo(0,0);
	keypad_4x4_wait();
	main_loop(); /* Enter main game loop */
	extra_deinit();
}

char extra_cback(void) {
	enable_extra = !enable_extra;
	return 0;
}
