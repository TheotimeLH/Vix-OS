.global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31 



#.macro ISR_NOERRCODE n
#	isr\n:
#		cli
#		push 0
#		push \n
#		jmp isr_common_stub
#.endm

#.macro ISR_ERRCODE n
#	isr\n:
#		cli
#		push \n
#		jmp isr_common_stub
#.endm


isr0:
	cli
	pushl 0
	pushl 0
	jmp isr_common_stub

isr1:
	cli
	pushl 0
	pushl 1
	jmp isr_common_stub

isr2:
	cli
	pushl 0
	pushl 2
	jmp isr_common_stub

isr3:
	cli
	pushl 0
	pushl 3
	jmp isr_common_stub

isr4:
	cli
	pushl 0
	pushl 4
	jmp isr_common_stub

isr5:
	cli
	pushl 0
	pushl 5
	jmp isr_common_stub

isr6:
	cli
	pushl 0
	pushl 6
	jmp isr_common_stub

isr7:
	cli
	pushl 0
	pushl 7
	jmp isr_common_stub

isr8:
	cli
	pushl 0
	pushl 8
	jmp isr_common_stub

isr9:
	cli
	pushl 0
	pushl 9
	jmp isr_common_stub

isr10:
	cli
	pushl 0
	pushl 10
	jmp isr_common_stub

isr11:
	cli
	pushl 0
	pushl 11
	jmp isr_common_stub

isr12:
	cli
	pushl 0
	pushl 12
	jmp isr_common_stub

isr13:
	cli
	pushl 0
	pushl 13
	jmp isr_common_stub

isr14:
	cli
	pushl 0
	pushl 14
	jmp isr_common_stub

isr15:
	cli
	pushl 0
	pushl 15
	jmp isr_common_stub

isr16:
	cli
	pushl 0
	pushl 16
	jmp isr_common_stub

isr17:
	cli
	pushl 0
	pushl 17
	jmp isr_common_stub

isr18:
	cli
	pushl 0
	pushl 18
	jmp isr_common_stub

isr19:
	cli
	pushl 0
	pushl 19
	jmp isr_common_stub

isr20:
	cli
	pushl 0
	pushl 20
	jmp isr_common_stub

isr21:
	cli
	pushl 0
	pushl 21
	jmp isr_common_stub

isr22:
	cli
	pushl 0
	pushl 22
	jmp isr_common_stub

isr23:
	cli
	pushl 0
	pushl 23
	jmp isr_common_stub

isr24:
	cli
	pushl 0
	pushl 24
	jmp isr_common_stub

isr25:
	cli
	pushl 0
	pushl 25
	jmp isr_common_stub

isr26:
	cli
	pushl 0
	pushl 26
	jmp isr_common_stub

isr27:
	cli
	pushl 0
	pushl 27
	jmp isr_common_stub

isr28:
	cli
	pushl 0
	pushl 28
	jmp isr_common_stub

isr29:
	cli
	pushl 0
	pushl 29
	jmp isr_common_stub

isr30:
	cli
	pushl 0
	pushl 30
	jmp isr_common_stub

isr31:
	cli
	pushl 0
	pushl 31
	jmp isr_common_stub

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

