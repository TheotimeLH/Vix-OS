#include "syscall.h"
#include "../common/timer.h"
#include "process.h"

static void syscall(registers_t regs)
{
    //rétablir le contexte du noyau ??
    uint32* eax=(uint32*)(regs.esp-4);
    switch (*eax)
    {
    case 0:
        *eax=get_ticks();
        break;
    case 1:
        if(regs.edi==0)
        {
            print_string((char*)regs.esi);
        }
        break;
    case 3:
        change_color(regs.edi>>7,regs.edi>>0);
        write_char(regs.edi>>24,regs.edi>>16&0xff,regs.edi>>8&0xff);
        break;
    default:
        break;
    }
}

void init_syscalls()
{
    register_interrupt_handler(0x42,&syscall);
}