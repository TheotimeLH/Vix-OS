
#include <string.h>

idt_gate_t idt_gates[256] ;
idtr_t idtr ;

static void idt_init()
{
	idtr.limit = sizeof(idt_gate_t)*256-1 ;
	idtr.base = (u32int) &idt_gates ;
	memset(&idt_gates, 0, idtr.limit+1) ;

	gate_set(0, (u32int)isr0, 0x08, 0x8E) ;
	gate_set(1, (u32int)isr1, 0x08, 0x8E) ;
	gate_set(2, (u32int)isr2, 0x08, 0x8E) ;
	gate_set(3, (u32int)isr3, 0x08, 0x8E) ;
	gate_set(4, (u32int)isr4, 0x08, 0x8E) ;
	gate_set(5, (u32int)isr5, 0x08, 0x8E) ;
	gate_set(6, (u32int)isr6, 0x08, 0x8E) ;
	gate_set(7, (u32int)isr7, 0x08, 0x8E) ;
	gate_set(8, (u32int)isr8, 0x08, 0x8E) ;
	gate_set(9, (u32int)isr9, 0x08, 0x8E) ;
	gate_set(10, (u32int)isr10, 0x08, 0x8E) ;
	gate_set(11, (u32int)isr11, 0x08, 0x8E) ;
	gate_set(12, (u32int)isr12, 0x08, 0x8E) ;
	gate_set(13, (u32int)isr13, 0x08, 0x8E) ;
	gate_set(14, (u32int)isr14, 0x08, 0x8E) ;
	gate_set(15, (u32int)isr15, 0x08, 0x8E) ;
	gate_set(16, (u32int)isr16, 0x08, 0x8E) ;
	gate_set(17, (u32int)isr17, 0x08, 0x8E) ;
	gate_set(18, (u32int)isr18, 0x08, 0x8E) ;
	gate_set(19, (u32int)isr19, 0x08, 0x8E) ;
	gate_set(20, (u32int)isr20, 0x08, 0x8E) ;
	gate_set(21, (u32int)isr21, 0x08, 0x8E) ;
	gate_set(22, (u32int)isr22, 0x08, 0x8E) ;
	gate_set(23, (u32int)isr23, 0x08, 0x8E) ;
	gate_set(24, (u32int)isr24, 0x08, 0x8E) ;
	gate_set(25, (u32int)isr25, 0x08, 0x8E) ;
	gate_set(26, (u32int)isr26, 0x08, 0x8E) ;
	gate_set(27, (u32int)isr27, 0x08, 0x8E) ;
	gate_set(28, (u32int)isr28, 0x08, 0x8E) ;
	gate_set(29, (u32int)isr29, 0x08, 0x8E) ;
	gate_set(30, (u32int)isr30, 0x08, 0x8E) ;
	gate_set(31, (u32int)isr31, 0x08, 0x8E) ;
}

static void gate_set(u8int n, u32int adrs, u16int slct, u8int flags)
{
	idt_entries[n].adrs = adrs ;
	idt_entries[n].slct = slct ;
	idt_entries[n].zeros = 0 ;
	idt_entries[n].flags = flags ;
}

