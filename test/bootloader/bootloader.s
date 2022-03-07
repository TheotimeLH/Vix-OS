begin:
	.code16
	mov	$0,%ax
	mov	%ax,%ds

	mov	$0x48,%ah
	mov	$(0x7c00+512),%si
	movw	$0x1E,(%si)
	int	$0x13

	jb	error

	mov	(0x7c00+512+0x18),%di
	call	print_int
	jmp	loop

error:
	mov	$0x666,%di
	call	print_int
	
	
	
	
loop:
	jmp loop

/*
	cli

	mov	$0,%ax
	mov	%ax,%ds
	
	lgdt	(gdt_desc)

	mov	%cr0,%eax
	or	$1,%eax
	mov	%eax,%cr0
	
	ljmp	$0x08,$clear_pipe
	
	.code32
clear_pipe:
	mov	$0x10,%ax
	mov	%ax,%ds
	mov	%ax,%ss


	mov 	$0x012345,%edi
	mov	$0x112345,%esi
	
	mov	%edi,(%edi)
	mov	%esi,(%esi)

	cmp	(%edi),%esi
	je	unset
	
	mov 	$0x41,%al
	jmp	set	

unset:
	mov	$0x42,%al
set:

	mov	$0x07,%ah
	mov	%ax,(0xB8000)
	
	
loop:
	jmp loop

char:.byte 0x42

gdt:
null:
	.quad 0
code:
	.word 0xFFFF
	.word 0
	.byte 0
	.byte 0b10011010
	.byte 0b11001111
	.byte 0

data:
	.word 0xFFFF
	.word 0
	.byte 0
	.byte 0b10010010
	.byte 0b11001111
	.byte 0
gdt_end:
gdt_desc:
	.word gdt_end-gdt
	.int gdt
*/

print_int:
	mov	$0x0E,%ah
	mov	$0x00,%bh
	mov	$0x07,%bl

	mov	%di,%cx
	and	$0xF000,%cx
	mov	%ch,%al
	shr	$4,%al
	call	hexa
	int	$0x10

	mov	%di,%cx
	and	$0x0F00,%cx
	mov	%ch,%al
	call	hexa
	int	$0x10

	mov	%di,%cx
	and	$0x00F0,%cx
	shr	$4,%cx
	mov	%cl,%al
	call	hexa
	int	$0x10

	mov	%di,%cx
	and	$0x000F,%cx
	mov	%cl,%al
	call	hexa
	int	$0x10
	
	ret

hexa:
	cmp	$9,%al
	jbe	dec
	add	$55,%al
	ret
dec:
	add	$0x30,%al
	ret

loc:
.space 	(510-(loc-begin)),0
.word 0xAA55
