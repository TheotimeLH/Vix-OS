#include "timer.h"
#include "isr.h"
#include "common.h"
#include "../video/vga_driver.h"

uint32 tick = 0;

static void timer_callback(registers_t regs){
	tick++;
	print_string("Tick: ");
	print_int(tick);
	print_new_line();
}

void init_timer(uint32 frequency){
	register_interrupt_handler(IRQ0, &timer_callback);
	uint32 divisor = 1193180 / frequency;
	outb(0x36, 0x43);
	uint8 l = (uint8) (divisor & 0xFF);
	uint8 h = (uint8) (divisor>>8 & 0xFF);
	outb(l, 0x40);
	outb(h, 0x40);
}
