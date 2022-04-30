#include "timer.h"
#include "isr.h"
#include "common.h"
#include "../video/vga_driver.h"

uint32 tick = 0;
uint32 freq = 0;

static void timer_callback(registers_t regs){
	tick++;
}

void wait(uint32 milliseconds){
	uint32 deb = tick;
	while(tick <= deb + (freq/1000)*milliseconds + 1);
}

inline uint32 get_ticks()
{
	return tick;
}

inline uint32 get_freq()
{
	return freq;
}

void init_timer(uint32 frequency){
	register_interrupt_handler(IRQ0, &timer_callback);
	asm volatile("cli");
	if(frequency>1193180)
		frequency=1193180;
	if(frequency<19)
		frequency=0;


	uint32 divisor =(frequency==0)?0: 1193180 / frequency;
	// print_string("Frequency : ");
	// print_int(frequency);
	// print_new_line();
	if(divisor==1)
		divisor=2;
	uint8 l = (uint8) (divisor & 0xFF);
	uint8 h = (uint8) (divisor>>8 & 0xFF);
	outb(0x36, 0x43);
	outb(l, 0x40);
	outb(h, 0x40);
	asm volatile("sti");

}
