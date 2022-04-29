#include "timer.h"
#include "isr.h"
#include "common.h"
#include "../video/vga_driver.h"

uint32 tick = 0;
bool seconde=false;

bool get_seconde()
{
	return seconde;
}

void set_seconde()
{
	seconde=false;
}

static void timer_callback(registers_t regs){
	tick++;
<<<<<<< HEAD
	if(tick%20==0)
	{
		print_string("Tick : ");
		print_int(tick);
		print_new_line();
	}
=======
	print_string("Tick: ");
	print_int(tick);
	print_new_line();
>>>>>>> a852b55c8fcf94fb047fae9a1e73c913268a647f
}

void init_timer(uint32 frequency){
	register_interrupt_handler(IRQ0, &timer_callback);
	asm volatile("cli");
	if(frequency>1193180)
		frequency=1193180;
	if(frequency<19)
		frequency=0;


	uint32 divisor =(frequency==0)?0: 1193180 / frequency;
	print_string("Frequency : ");
	print_int(frequency);
	print_new_line();
	outb(0x36, 0x43);
	if(divisor==1)
		divisor=2;
	uint8 l = (uint8) (divisor & 0xFF);
	uint8 h = (uint8) (divisor>>8 & 0xFF);
	outb(l, 0x40);
	outb(h, 0x40);
	asm volatile("sti");

}
