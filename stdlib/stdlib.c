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
    asm volatile("push %esi");
    asm volatile("push %eax");
    asm volatile("push %edi");

    asm volatile("mov %0,%%esi":"=m"(str));
    asm volatile("mov $1,%eax");
    asm volatile("mov %0,%%edi":"=m"(file_desc));
    asm volatile("int $0x42");

    asm volatile("pop %edi");
    asm volatile("pop %eax");
    asm volatile("pop %esi");
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
