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
	push $0
	push $0
	jmp isr_common_stub

isr1:
	cli
	push $0
	push $1
	jmp isr_common_stub

isr2:
	cli
	push $0
	push $2
	jmp isr_common_stub

isr3:
	cli
	push $0
	push $3
	jmp isr_common_stub

isr4:
	cli
	push $0
	push $4
	jmp isr_common_stub

isr5:
	cli
	push $0
	push $5
	jmp isr_common_stub

isr6:
	cli
	push $0
	push $6
	jmp isr_common_stub

isr7:
	cli
	push $0
	push $7
	jmp isr_common_stub

isr8:
	cli
	#push 0
	push $8
	jmp isr_common_stub

isr9:
	cli
	push $0
	push $9
	jmp isr_common_stub

isr10:
	cli
	#push 0
	push $10
	jmp isr_common_stub

isr11:
	cli
	#push 0
	push $11
	jmp isr_common_stub

isr12:
	cli
	#push 0
	push $12
	jmp isr_common_stub

isr13:
	cli
	#push 0
	push $13
	jmp isr_common_stub

isr14:
	cli
	#push 0
	push $14
	jmp isr_common_stub

isr15:
	cli
	push $0
	push $15
	jmp isr_common_stub

isr16:
	cli
	push $0
	push $16
	jmp isr_common_stub

isr17:
	cli
	push $0
	push $17
	jmp isr_common_stub

isr18:
	cli
	push $0
	push $18
	jmp isr_common_stub

isr19:
	cli
	push $0
	push $19
	jmp isr_common_stub

isr20:
	cli
	push $0
	push $20
	jmp isr_common_stub

isr21:
	cli
	push $0
	push $21
	jmp isr_common_stub

isr22:
	cli
	push $0
	push $22
	jmp isr_common_stub

isr23:
	cli
	push $0
	push $23
	jmp isr_common_stub

isr24:
	cli
	push $0
	push $24
	jmp isr_common_stub

isr25:
	cli
	push $0
	push $25
	jmp isr_common_stub

isr26:
	cli
	push $0
	push $26
	jmp isr_common_stub

isr27:
	cli
	push $0
	push $27
	jmp isr_common_stub

isr28:
	cli
	push $0
	push $28
	jmp isr_common_stub

isr29:
	cli
	push $0
	push $29
	jmp isr_common_stub

isr30:
	cli
	push $0
	push $30
	jmp isr_common_stub

isr31:
	cli
	push $0
	push $31
	jmp isr_common_stub

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

