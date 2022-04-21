#ifndef VGA_H
#define VGA_H
#include "../common/common.h"

#define VGA_ADDRESS_TEXT 0xB8000
#define BUFSIZE 2200

#define TEXT_WIDTH 80
#define TEXT_HEIGHT 25 

#ifdef __cplusplus
extern "C" {
#endif

uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color);
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color);
void init_vga(uint8 fore_color, uint8 back_color);
void print_new_line();
void print_char(char ch);
void print_hexa(uint32 x);
void print_string(char *str);
void print_int(int num);
void write_char(int x, int y, char c);
void change_color(uint8 fore_color, uint8 back_color);
void scroll();

#ifdef __cplusplus
}
#endif


enum vga_mode {
	TEXT,
	GRAPHIC,
};

enum vga_color {
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
};


#endif 
