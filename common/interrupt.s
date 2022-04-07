.macro ISR_NOERRCODE n
	isr\n:
		cli
		push 0
		push \n
		jmp isr_common_stub
.endm

.macro ISR_ERRCODE n
	isr\n:
		cli
		push \n
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
ISR_NOERRCODE 8
ISR_NOERRCODE 9
ISR_NOERRCODE 10
ISR_NOERRCODE 11
ISR_NOERRCODE 12
ISR_NOERRCODE 13
ISR_NOERRCODE 14
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

isr_common_stub:
	pusha
	#push %rdi
	#push %rsi
	#push %rbp
	#push %rsp
	#push %rbx
	#push %rdx
	#push %rcx
	#push %rax
	mov %ds, %ax
	push %eax

	mov 0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	popa
	#pop %rax
	#pop %rcx
	#pop %rdx
	#pop %rbx
	#pop %rsp
	#pop %rbp
	#pop %rsi
	#pop %rdi

	add $8, %esp
	sti
	iret

