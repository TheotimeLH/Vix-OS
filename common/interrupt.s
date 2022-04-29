.macro ISR_NOERRCODE n
	.global isr\n
	isr\n:
		cli
		push $0
		push $\n
		jmp isr_common_stub
.endm

.macro ISR_ERRCODE n
	.global isr\n
	isr\n:
		cli
		push $\n
		jmp isr_common_stub
.endm

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 66

.macro IRQ n m # On fait un macro pour tout les irq ( ce qu'on aurait du faire pour les isr )
	.global irq\n
	irq\n:
		cli
		push $0
		push $\m
		jmp irq_common_stub
.endm

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47


isr_common_stub:
	pusha
	mov %ds, %ax
	push %eax

	mov $0x10, %ax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs

	call isr_handler
	
	pop %eax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs

	popa
	add $0x8, %esp
	sti
	iret

irq_common_stub: # Pour gerer les irq
	pusha

	mov %ds, %ax
	push %eax

	mov $0x10, %ax
	mov %eax, %ds
	mov %eax, %es
	mov %eax, %fs
	mov %eax, %gs

	call irq_handler

	pop %ebx
	mov %ebx, %ds
	mov %ebx, %es
	mov %ebx, %fs
	mov %ebx, %gs

	popa
	add $0x8, %esp
	sti
	iret

