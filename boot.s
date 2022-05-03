# set magic number to 0x1BADB002 to identified by bootloader 
.set MAGIC,    0x1BADB002

# set flags to 0
.set FLAGS,    0

# set the checksum
.set CHECKSUM, -(MAGIC + FLAGS)

# set multiboot enabled
.section .multiboot

# define type to long for each data defined as above
.long MAGIC
.long FLAGS
.long CHECKSUM


# set the stack bottom 
stackBottom:

# define the maximum size of stack to 512 bytes
.skip 0x100000


# set the stack top which grows from higher to lower
stackTop:

.skip 0x10000

.global kernel_stack
kernel_stack:

.section .text
.global _start
.type _start, @function


_start:

  # assign current stack pointer location to stackTop
	mov $stackTop, %esp

  # call the kernel main source
	push %eax
	push %ebx
	call kernel_main 

	cli


# put system in infinite loop
hltLoop:

	hlt
	jmp hltLoop

.global return_proc
return_proc:
	mov %ebx,%esp
	pop %ebx
	pop %esi
	pop %ebp
	ret

.size _start, . - _start
