#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "../common/common.h"
#include "../common/isr.h"
#include "../fat_driver/fat_driver.h"

#define get_uint32(buff,addr) *(uint32*)(&buff[addr])
#define get_uint16(buff,addr) *(uint16*)(&buff[addr])

typedef enum proc_state
{
    WAITING,RUNNABLE,ZOMBIE,FREE
} proc_state;

typedef struct process
{
    uint32 ppid;
    proc_state state;
    registers_t saved_context;
    uint32 pg_entry;
} process;

void init_process_tab();
uint32 load_process(uint8* elf,uint32 current_pid=0);
void run_process(uint32 pid);

#endif