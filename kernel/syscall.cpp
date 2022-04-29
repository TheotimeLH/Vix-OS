#include "syscall.h"
#include "../common/timer.h"
#include "process.h"

static void syscall(registers_t regs)
{
    //rétablir le contexte du noyau ??
    print_hexa(*(uint32*)(regs.esp-4));
}

void init_syscalls()
{
    register_interrupt_handler(0x42,&syscall);
}