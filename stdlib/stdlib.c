#include "stdlib.h"

uint32 get_ticks()
{
    uint32 tick;

    asm volatile("mov $0,%eax");
    asm volatile("int $0x42");
    asm volatile("mov %%eax,%0"::"m"(tick));

    return tick;
}

void write(uint32 file_desc,char *str)
{
    fwrite(file_desc,(uint8*)str,strlen(str));
}

void fwrite(uint32 file_desc,uint8 *data,uint32 size)
{
    asm volatile("push %esi");
    asm volatile("push %eax");
    asm volatile("push %edi");
    asm volatile("push %ebx");

    asm volatile("mov %0,%%esi":"=m"(data));
    asm volatile("mov %0,%%ebx":"=m"(size));
    asm volatile("mov $1,%eax");
    asm volatile("mov %0,%%edi":"=m"(file_desc));
    asm volatile("int $0x42");

    asm volatile("pop %ebx");
    asm volatile("pop %edi");
    asm volatile("pop %eax");
    asm volatile("pop %esi");
}

uint32 read(uint32 file,uint8* buff,uint32 size)
{
    uint32 ret;

    asm volatile("push %esi");
    asm volatile("push %ebx");
    asm volatile("push %edi");


    asm volatile("mov %0,%%edi":"=m"(file));
    asm volatile("mov %0,%%esi":"=m"(buff));
    asm volatile("mov %0,%%ebx":"=m"(size));
    asm volatile("mov $2,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %edi");
    asm volatile("pop %ebx");
    asm volatile("pop %esi");

    asm volatile("mov %%eax,%0"::"m"(ret));

    return ret;
}

void print_screen(uint32 x,uint32 y,char car,uint8 fg_color,uint8 bg_color)
{
    asm volatile("push %edi");
    asm volatile("push %eax");
    if(x>80)
        x=80;
    if(y>25)
        y=25;
    uint32 edi=(x<<24)|(y<<16)|(((uint32)car)<<8)|(((uint32)fg_color&0xF)<<4)|((uint32)bg_color&0xF);
    asm volatile("mov %0,%%edi":"=m"(edi));
    asm volatile("mov $3,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %eax");
    asm volatile("pop %edi");
}

keyboard_t get_keyboard()
{
    uint32 eax;
    asm volatile("mov $4,%eax");
    asm volatile("int $0x42");
    asm volatile("mov %%eax,%0"::"m"(eax));
    keyboard_t ret;
    ret.type=eax&0xFF;
    eax=eax>>8;
    ret.k=*(union key*)(&eax);

    return ret;
}

uint32 open(char* filename)
{
    uint32 ret;
    asm volatile("push %edi");

    asm volatile("mov %0,%%edi":"=m"(filename));
    asm volatile("mov $5,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %edi");

    asm volatile("mov %%eax,%0"::"m"(ret));
    return ret;
}

void exec(char* filename)
{
    asm volatile("push %edi");
    asm volatile("push %eax");

    asm volatile("mov %0,%%edi":"=m"(filename));
    asm volatile("mov $6,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %eax");
    asm volatile("pop %edi");
}

uint32 get_pid()
{
    uint32 pid;

    asm volatile("mov $7,%eax");
    asm volatile("int $0x42");
    asm volatile("mov %%eax,%0"::"m"(pid));

    return pid;
}

uint32 strlen(char* str)
{
	uint32 ret;
	for(ret=0;str[ret]!=0;ret++);
	return ret;
}