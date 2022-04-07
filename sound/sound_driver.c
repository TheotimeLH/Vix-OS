#include "sound_driver.h"
#include "../common/common.h"
#include "../common/idt.h"

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

void sound_blaster(){
	idt_init(); // initialisation de la table, mais il faudrait le faire plus tôt, je sais pas trop comment ça marche
	// On reset le port
	outb(1, 0x226);
	asm("mov 0x86, ah");
	asm("mov 0x0000, cx");
	asm("mov 0xFFFF, dx");
	isr21();
	outb(0, 0x226);
	// On allume les hauts-parleurs
	outb(0xd1, 0x22c);
	//DMA Channel 1
	outb(5, 0x0a); // disable channel 1
	outb(1, 0x0c); // flip flop
	outb(0x49, 0x0b); // transfer mode
	outb(0x01, 0x83); // PAGE TRANSFER
	outb(0x04, 0x02); // position low bit
	outb(0x0f, 0x02);
	outb(0xFF, 0x03);
	outb(0x0F, 0x03);
	outb(1, 0x0a);

	// program sound blaster 16
	outb(0x40, 0x22c); // set time constant
	outb(165, 0x22c); // 10989 Hz
	outb(0xc0, 0x22c); // 8 bit sound
	outb(0x00, 0x22c); // mono and unsigned sound data
	outb(0xfe, 0x22c); 
	outb(0x0f, 0x22c);
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
	sound_blaster();
	
}

