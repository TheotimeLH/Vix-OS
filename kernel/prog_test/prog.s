.section .bss
.align 16
stack_bottom:
.skip 0x10000
stack_top:
 
.section .text
.global _start
_start:
	mov $stack_top, %esp

	call main

1:
	jmp 1b

.size _start, . - _start
