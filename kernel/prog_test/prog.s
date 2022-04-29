.section .bss
.align 16
stack_bottom:
.skip 0x10000
stack_top:
 
.section .text
.global _start
_start:
	mov $stack_top, %esp

	movw $0x0742,(0xB8000)
	mov $0x42694269,%eax

	int $0x42
	int $0x42
 

1:
	jmp 1b
 
.size _start, . - _start
