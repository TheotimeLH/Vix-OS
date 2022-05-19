.section .bss
.align 16
stack_bottom:
.skip 0x2000000
stack_top:
 
.section .text
.global _start
_start:
	mov $stack_top, %esp

	call crt0_main

1:
	jmp 1b

.size _start, . - _start
