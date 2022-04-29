//.section .bss
//.align 16
//stack_bottom:
//.skip 0x10000
//stack_top:
 
.section .text
.global _start
_start:
//	mov $stack_top, %esp

	mov $0xB8000,%edi
	movw $0x4270,(%edi)

	int $0x3
	movw $0x4270,2(%edi)
 

1:
	jmp 1b
 
.size _start, . - _start
