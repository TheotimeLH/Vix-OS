#ifndef STDLIB_H
#define STDLIB_H

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

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
void print_screen(uint32 x,uint32 y,char car,uint8 fg_color,uint8 bg_color);

#endif
