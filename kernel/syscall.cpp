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
    default:
        break;
    }
}

void init_syscalls()
{
    register_interrupt_handler(0x42,&syscall);
}