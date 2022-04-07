
#include <stdint.h>

struct idt_gate_s
{
		uint16_t ofs1 ; // Première moitié de l'adresse
		uint16_t slct ; // Segment selector
		uint8_t zeros ; // Reserved
		uint8_t flags ; // Gate Type (4) / 0 / Privilege Level (2) / Present (1)
		uint16_t ofs2 ; // Seconde moitié de l'adresse
} __attribute__((packed)) ;

typedef struct idt_gate_s idt_gate_t ;

struct idtr_s
{
	uint16_t limit ; // Dernier offset valide dans la table
	uint32_t base ; // Adresse de la première entrée
} __attribute__((packed)) ;

typedef struct idtr_s idtr_t ;

void idt_init();

extern void isr0() ;
extern void isr1() ;
extern void isr2() ;
extern void isr3() ;
extern void isr4() ;
extern void isr5() ;
extern void isr6() ;
extern void isr7() ;
extern void isr8() ;
extern void isr9() ;
extern void isr10() ;
extern void isr11() ;
extern void isr12() ;
extern void isr13() ;
extern void isr14() ;
extern void isr15() ;
extern void isr16() ;
extern void isr17() ;
extern void isr18() ;
extern void isr19() ;
extern void isr20() ;
extern void isr21() ;
extern void isr22() ;
extern void isr23() ;
extern void isr24() ;
extern void isr25() ;
extern void isr26() ;
extern void isr27() ;
extern void isr28() ;
extern void isr29() ;
extern void isr30() ;
extern void isr31() ;

