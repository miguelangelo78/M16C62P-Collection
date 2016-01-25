#include "globals.h"

int printf(const char _far fmt[], ...) {
	char printf_buff[128];
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = vsprintf(printf_buff, fmt, args);
	va_end(args);
	puts(printf_buff);
	return ret;
}

/* Prints from left to right on a certain location */
int printf_at(char * fmt, unsigned char x, unsigned char y, ...) {
	Point p;
	char printf_at_buff[128];
	int ret;
	va_list args;
	va_start(args, fmt);
	va_arg(args, unsigned char); va_arg(args, unsigned char); /* Iterate to skip the 2nd and 3rd arguments */
	ret = vsprintf(printf_at_buff, fmt, args);
	va_end(args);
	p = GLCD_GoTo(x, y);
	puts(printf_at_buff);
	GLCD_GoTo(p.x, p.y);
	return ret;
}

/* Prints from right to left on a certain location. 
   Useful for printing on the right corner of the screen */
int printf_at_rl(char * fmt, unsigned char x, unsigned char y, ...) {
	Point p;
	char printf_at_buff[128];
	int ret;
	va_list args;
	va_start(args, fmt);
	va_arg(args, unsigned char); va_arg(args, unsigned char); /* Iterate to skip the 2nd and 3rd arguments */
	ret = vsprintf(printf_at_buff, fmt, args);
	va_end(args);
	p = GLCD_GoTo(x - strlen(printf_at_buff) * GLCD_CHAR_WIDTH + GLCD_CHAR_WIDTH, y);
	puts(printf_at_buff);
	GLCD_GoTo(p.x, p.y);
	return ret;
}

#define NULLPROG (program_t)0
#define SUBMENU NULLPROG

typedef int (*program_t)(void); /* Program callback typedef */

typedef struct menu_option {
	char * option_name;
	struct menu * submenu; /* If null, then this option is an end node */
	program_t prog_callback; /* If null, then this option is a submenu */
	/* If both are null, the option should be disabled */
} menu_option_t;

typedef struct menu {
	char * menu_name;
	struct menu * parent_menu;
	menu_option_t ** options;
	int options_count;
} menu_t;

menu_t * menu_root = 0;
menu_t * current_menu;

menu_option_t * menu_option_create(char * option_name, program_t cback) {
	menu_option_t * new_option = malloc(sizeof(menu_option_t));
	new_option->option_name = option_name;
	new_option->submenu = 0; /* Will link later when a menu gets created */
	new_option->prog_callback = cback ? cback : 0; /* Ternary if to check if cback is null or not */
	return new_option;
}

menu_option_t ** create_options(int option_count, ...) {
	int i;
	va_list args;
	menu_option_t ** new_option_list = malloc(sizeof(menu_option_t*) * (option_count + 1));
	va_start(args, option_count);
	for(i = 0; i < option_count; i++) {
		char * str = va_arg(args, char*);
		program_t cback = va_arg(args, program_t);
		new_option_list[i] = menu_option_create(str, cback);
	}
	new_option_list[i] = 0; /* One extra as a null pointer, just so we can count how many options there are */
	va_end(args);
	return new_option_list;
}

/* Params: menu option node, and a list of menu options */
menu_t * menu_create(menu_t* parent, menu_option_t * menu_node_option, menu_option_t ** options) {
	int i = 0;
	menu_t * new_menu = malloc(sizeof(menu_t));
	new_menu->options = options;
	while(options[i]) i++; /* Count how many options were provided */
	new_menu->options_count = i;
	new_menu->menu_name = malloc(sizeof(char)*32);
	if(!menu_node_option) /* If parent node is null, then the name shall be Main Menu */
		strcpy(new_menu->menu_name, "Main Menu");
	else /* Else grab the name from the option from the parent: */
		strcpy(new_menu->menu_name, menu_node_option->option_name);
	
	/* Add this new menu to the node 'menu_node_option' */
	if(menu_node_option && parent) {
		new_menu->parent_menu = parent; 
		menu_node_option->submenu = new_menu;
	} else {
		new_menu->parent_menu = menu_node_option->submenu = 0;
	}
	
	return new_menu;
}

/* Updates the menu name position according to the offset provided: */
void animate_menu_name(menu_t * current_menu, int offset) {
	int i;
	GLCD_GoTo(0, 0);
	for(i = 0; i <= (GLCD_CHARS_HORIZ-strlen(current_menu->menu_name))/2 - (offset+1); i++)
		putc('*');
	printf(" %s ", current_menu->menu_name);
	while(1)
		if(GLCD_getXY().x <= GLCD_WIDTH - 2)
			putc('*');
		else 
			break;
	GLCD_GoTo(0, 2);
}

void menu_display_options(menu_t * current_menu, char cursor) {
	int i;
	Point p;
	for(i = 0; i < current_menu->options_count; i++) {
		menu_option_t * option = current_menu->options[i];
		if(!option->submenu && !option->prog_callback) continue; /* Invalid option (no submenu AND no program) */
		printf(" %c %s\n", i==cursor?'>':'-', option->option_name);
		p = GLCD_getXY();
		if(option->submenu) {
			/* It's a menu */
			GLCD_GoTo(GLCD_WIDTH-19, p.y-1);
			puts("...");	
		} else {
			/* It's a program */	
			GLCD_GoTo(GLCD_WIDTH-8, p.y-1);
			puts(">");	
		}
		GLCD_GoTo(p.x, p.y);
	}
}

void menu_draw_borders(void) {
	int i=0;
	/* Draw bottom border: */
	printf_at("_____________________", 0, GLCD_HEIGHT-1);
	/* Draw left border: */
	GLCD_Rectangle(0,8,1,70);
	/* Draw right border: */
	GLCD_Rectangle(126,8,127,70);
}

void menu_navigate(void) /* Aka menu/program navigator */
{
	Point p;
	int i, ret = 0;
	char cursor = 0;
	char update_menu = 0;
	char menu_name_offset_dir = 0; /* Update menu left or right */
	int menu_name_offset = 0, delay = 0; /* Offset of menu from the center, and delay for updating the menu name */
	int menu_name_bounds = 0; /* How far the name goes to the sides */
	for(;;) {
		/************* Display current menu section: *********************/
		GLCD_ClearScreen();
		GLCD_GoTo(0, 0);
		/* Print menu name aligned on center */
		animate_menu_name(current_menu, menu_name_offset);
		menu_name_bounds = 10-strlen(current_menu->menu_name); /* Necessary for the animation of the menu */
		/* Display options: */
		menu_display_options(current_menu, cursor);
		/* Draw back button: */
		if(current_menu->parent_menu)
			printf_at_rl("%c-Back", GLCD_WIDTH - 7, GLCD_HEIGHT - 2, BACK);
		/* Show return code from recently ran program: */
		if(ret) {
			printf_at("ret=%d", 3, GLCD_HEIGHT-2, ret);
			ret = 0;
		}
		/* Draw borders: */
		menu_draw_borders();
		
		/************** Keypad input section: *************************** */
		keypad_4x4_wait(); /* Wait for keypad to cool down its buffer value. When it outputs a 0 then it's ready */
		update_menu = 0;
		while(!update_menu) {
			/* Update menu name position: */
			if(delay++>20000) {
				delay = 0;
				if(menu_name_offset_dir) {
					menu_name_offset--;
					if(menu_name_offset < -menu_name_bounds) menu_name_offset_dir = 0;
				}
				else {
					menu_name_offset++;
					if(menu_name_offset > menu_name_bounds + 1) menu_name_offset_dir = 1;
				}
				animate_menu_name(current_menu, menu_name_offset);
			}
				
			/* Read keypad: */
			switch(getcommand()){
				case OPEN:
					if(current_menu->options[cursor]->submenu || current_menu->options[cursor]->prog_callback) {
						/* Jump to either submenu or program: */
						if(current_menu->options[cursor]->prog_callback) { /* Execute program */
							GLCD_ClearScreen();
							p = GLCD_GoTo(0, 0);
							ret = current_menu->options[cursor]->prog_callback();
							/* Cleanup everything from the program we just returned from: */
							GLCD_GoTo(p.x, p.y);
							uninstall_all_cback();
						}
						else { /* Open submenu */
							current_menu = current_menu->options[cursor]->submenu;
						}
						menu_name_offset = 0;
						cursor = 0;
						update_menu = 1;
					}
					break;
				case BACK:
					if(current_menu->parent_menu) {
						current_menu = current_menu->parent_menu;
						cursor = 0;
						update_menu = 1;
						menu_name_offset = 0;
					}
					break;
				case UP: 
					if(cursor > 0) {
						cursor--; 
						update_menu = 1; 
					}
					break;
				case DOWN:
					if(cursor < current_menu->options_count-1) {
						cursor++; 
						update_menu = 1; 
					}
					break;
				case 0: continue; /* No key pressed  */
			}
		}
	}
}

void init_menu_tree(void) {
	/* Set root branch with initial child branches */
	menu_root = menu_create(0, 0, create_options(5, "GPIO", SUBMENU, "GLCD & Keypad", SUBMENU, "Timers", SUBMENU, "Info", info, "Extra", extra));
	
	/* Add all options to the menu tree */
	menu_create(menu_root, menu_root->options[0], create_options(2, "LEDs/7-segment", led_7seg, "Interrupts", led_pattern));
	menu_create(menu_root, menu_root->options[1], create_options(2, "Keypad Scan", keypad_scan, "Key Selection", keypad_selection));
	menu_create(menu_root, menu_root->options[2], create_options(2, "Set timer/counter", timer_set, "Cascade timers", timer_cascade_int));
	
	current_menu = menu_root; /* Set initial current menu */
}

/* Show program intro explaining navigation controls: */
void intro(void) {
	GLCD_Rectangle(0, 0, GLCD_WIDTH+5, 5);
	GLCD_WriteString("\n\n    NG2S901\n  Assignment 1\n", Font_System7x8);
	printf("Use keys %c,%c,%c and %c\nto navigate.", UP, DOWN, BACK, OPEN);
	printf_at("Press any key!", GLCD_WIDTH/2-40, GLCD_HEIGHT-1);
	GLCD_Rectangle(0, 48, GLCD_WIDTH+5, 48);
	read_key();
}

void main(void)
{
	Initialise();
	GLCD_Initialise();
	keyscan_4x4_init();
	init_isr();
	init_menu_tree();
	intro();
	menu_navigate();
	for(;;);
}
