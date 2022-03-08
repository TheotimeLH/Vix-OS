#ifndef VGA_H
#define VGA_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define VGA_ADDRESS_TEXT 0xB8000
#define BUFSIZE 2200

#define VGA_ADDRESS_GRAPHIC 0xA0000
#define BUFSIZE_PIX 64000

#define TEXT_WIDTH 80
#define TEXT_HEIGHT 55

#define GRAPHIC_WIDTH 80
#define GRAPHIC_HEIGHT 55

uint16* vga_buffer_text; // Le buffer chargé des caracteres

uint16* vga_buffer_graphic;

#define NULL 0

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
