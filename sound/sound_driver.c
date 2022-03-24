#include "sound_driver.h"
#include "../common/common.h"


void play_sound(uint32 freq)
{
	uint32 Div;
	uint8 tmp;
	Div = 1193180 / freq;
	outb(0x43, 0xb6);
	outb(0x42, (uint8) Div);
	outb(0x42, (uint8) (Div >> 8));

	tmp = inb(0x61);

	if(tmp != (tmp | 3)){
		outb(0x61, tmp|3);
	}
}

void no_sound()
{
	uint8 tmp  = inb(0x61) & 0xFC;
	outb(0x61, tmp);
}

void sound_blaster_test(){
}

void beep()
{
	play_sound(1000);
	//timer_wait(10);
	//no_sound();
	//no_sound();
}

void test_driver(){
	#include "../video/vga_driver.h"
	init_vga(RED, BLUE);
	print_new_line();
	print_string("TEST DU SON");
	beep();
	
}

