#ifndef STDLIB_H
#define STDLIB_H

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define STDLIB_VERSION
#include "../common/keyboard.h"
#undef STDLIB_VERSION

typedef enum vga_color {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	GREY,
	DARK_GREY,
	BRIGHT_BLUE,
	BRIGHT_GREEN,
	BRIGHT_CYAN,
	BRIGHT_RED,
	BRIGHT_MAGENTA,
	YELLOW,
	WHITE,
} vga_color;

uint32 get_ticks();
void write(uint32 file_desc,char *str);
uint32 read(uint32 file,uint8* buff,uint32 size);
void print_screen(uint32 x,uint32 y,char car,uint8 fg_color,uint8 bg_color);
keyboard_t get_keyboard();
uint32 open(char* filename);

#endif
