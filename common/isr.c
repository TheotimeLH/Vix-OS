#include "common.h"
#include "isr.h"
#include "../video/vga_driver.h"

isr_t interrupt_handlers[256];

void isr_handler(registers_t regs){
	print_string("received interrupt :");
	print_int(regs.int_no);
	print_new_line();
	//print_string("error code :");
	//print_int(regs.err_code);
}


void irq_handler(registers_t regs){
	if(regs.int_no >= 40){
		outb(0x20, 0xA0);
	}
	outb(0x20, 0x20);
	if(interrupt_handlers[regs.int_no] != 0){
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else{
		print_string("Unhandled IRQ number : ");
		print_int(regs.int_no);
		print_new_line();
	}
}

void register_interrupt_handler(uint8 n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}
