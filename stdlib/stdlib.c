#include "stdlib.h"

uint32 digit_count(int num)
{
	uint32 count = 0;
	if(num == 0)
		return 1;
	else
	{
		while(num)
		{
			count++;
			num /= 10;
		}
		return count;
	}
}

void itoa(int num, char* number)
{
	int dg = digit_count(num);
	int i = dg-1;
	if(num == 0 && dg == 1)
	{
		number[0] = '0';
		number[1] = '\0';
	}
	else
	{
		while(num)
		{
			char x = num%10;
			number[i--] = x +'0';
			num /= 10;
		}
		number[dg] = '\0';
	}
}

void putchar(char c)
{
	char str[2];
	str[0] = c;
	str[1] = '\0';
	write(0, str);
}

void print_int(int num)
{
	char* str[digit_count(num) + 1];
	itoa(num, str);
	write(0, str);
}

void print_hexa(uint32 x)
{
	for(int i=7;i>=0;i--)
	{
		uint32 c=(x&(0xF<<(i*4)))>>(i*4);
		if(c>9)
			putchar(c+0x41-10);
		else
			putchar(c+0x30);
	}
}


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

void exit(uint32 i)
{
    asm volatile("push %edi");
    asm volatile("mov $8,%eax");
    asm volatile("mov %0,%%edi":"=m"(i));
    asm volatile("int $0x42");
    asm volatile("pop %edi");
}

uint32 wait(uint32* status)
{
    asm volatile("push %ebx");

    int sys_wait_ret;

    do
    {
        asm volatile("mov $9,%eax");
        asm volatile("int $0x42");
        asm volatile("mov %%eax,%0"::"m"(sys_wait_ret));
        asm volatile("mov %%ebx,%0"::"m"(*status));
    }while (sys_wait_ret==-2);

    asm volatile("pop %ebx");

    return sys_wait_ret;
}

uint32 get_ppid()
{
    uint32 ppid;

    asm volatile("mov $0xA,%eax");
    asm volatile("int $0x42");
    asm volatile("mov %%eax,%0"::"m"(ppid));

    return ppid;

}

uint32 change_directory(char* name)
{
    uint32 ret;
    asm volatile("push %edi");

    asm volatile("mov %0,%%edi":"=m"(name));
    asm volatile("mov $0xB,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %edi");

    asm volatile("mov %%eax,%0"::"m"(ret));
    return ret;
}

uint32 list_entries(char* buff,uint32 n_entries)
{
    uint32 ret;

    asm volatile("push %esi");
    asm volatile("push %edi");


    asm volatile("mov %0,%%edi":"=m"(buff));
    asm volatile("mov %0,%%esi":"=m"(n_entries));
    asm volatile("mov $0xC,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %edi");
    asm volatile("pop %esi");

    asm volatile("mov %%eax,%0"::"m"(ret));

    return ret;
}

uint32 make_directory(char* name)
{
    uint32 ret;
    asm volatile("push %edi");

    asm volatile("mov %0,%%edi":"=m"(name));
    asm volatile("mov $0xD,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %edi");

    asm volatile("mov %%eax,%0"::"m"(ret));
    return ret;
}

uint32 remove_entry(char* name)
{
    uint32 ret;
    asm volatile("push %edi");

    asm volatile("mov %0,%%edi":"=m"(name));
    asm volatile("mov $0xE,%eax");
    asm volatile("int $0x42");

    asm volatile("pop %edi");

    asm volatile("mov %%eax,%0"::"m"(ret));
    return ret;
}

uint32 strlen(char* str)
{
	uint32 ret;
	for(ret=0;str[ret]!=0;ret++);
	return ret;
}
