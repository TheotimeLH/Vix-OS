#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "ata_driver.h"
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint16_t* terminal_buffer;


void kernel_main() 
{
//	terminal_initialize();
	init_vga(0x07,0x00);

  	Drive_id id=ata_identify(hda);
  	print_int(id.taille_secteur);
	print_new_line();
  	print_int(id.nb_secteur);
	print_new_line();

	uint16_t buff[512];
	buff[0]=0x4242;

	while(!ata_read(hda,2,0,buff));
	print_hexa(buff[0]);
	print_new_line();

	print_hexa(buff[0x3F0/2]);
	print_new_line();

	while(1);
}
