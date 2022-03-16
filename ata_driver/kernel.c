#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "ata_driver.h"
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint16_t* terminal_buffer;
 
void terminal_initialize() 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = 0x0720;
		}
	}
}
 
void terminal_putchar(char c) 
{
	terminal_buffer[terminal_row*VGA_WIDTH+terminal_column] = (uint16_t) c | 0x0700; 
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_newline()
{
	terminal_column=0;
	if(++terminal_row==VGA_HEIGHT)
		terminal_row=0;
}

void print_int(uint32_t x)
{
	for(int i=7;i>=0;i--)
	{
		uint32_t c=(x&(0xF<<(i*4)))>>(i*4);
		if(c>9)
			terminal_putchar(c+0x41-10);
		else
			terminal_putchar(c+0x30);
	}
}

void kernel_main() 
{
	terminal_initialize();

	uint16_t buffer[256];
	print_int(ata_read(hda,1,0,buffer));
	terminal_newline();
	print_int(((uint32_t*)buffer)[0]);

}
