#include "process.h"

const uint32 n_process=10;
process tab_process[n_process];


void init_process_tab()
{
    for(int i=0;i<n_process;i++)
    {
        tab_process[i].state=FREE;
    }
}

uint32 load_process(uint8* elf,uint32 size,uint32 current_pid)
{
    uint32 pid;
    for(pid=0;pid<n_process;pid++)
    {
        if(tab_process[pid].state==FREE)
            break;
    }
    if(pid==n_process)
    {
        print_string("tous les processus pris\n");
        return pid;
    }

    char elf_sign[4]="ELF";
    bool is_elf=true;
    for(int i=0;i<3;i++)
    {
        is_elf=is_elf&&(elf_sign[i]==elf[i+1]);
    }
    if(!(elf[0]==0x7F&&is_elf))
    {
        print_string("erreur de format\n");
        return n_process;
    }
    process* proc=&tab_process[pid];
    
    proc->pg_entry=get_uint32(elf,0x18);
    proc->ppid=current_pid;
    proc->saved_context.ds=0x10;
    proc->saved_context.cs=0x10;
    proc->saved_context.ss=0x10;
    proc->saved_context.eip=proc->pg_entry;
    
    uint32 pg_header=get_uint32(elf,0x1C);
    uint16 n_entry=get_uint16(elf,0x2C);
    for(uint16 i=0;i<n_entry;i++)
    {
        uint32 seg_header=pg_header+i*32;
        if(get_uint32(elf,seg_header)!=1)
        {
            continue;

        }
        uint32 size_in_file=get_uint32(elf,seg_header+0x10);
        uint32 size_in_mem=get_uint32(elf,seg_header+0x14);
        uint8* seg_in_mem=(uint8*)get_uint32(elf,seg_header+0x8);
        uint8* seg_in_file=elf+get_uint32(elf,seg_header+0x4);
        for(uint32 j=0;j<size_in_mem;j++)
        {
            if(j<size_in_file)
            {
                seg_in_mem[j]=seg_in_file[j];
            }
            else
            {
                seg_in_mem[j]=0;
            }
        }
    }
    proc->state=RUNNABLE;
    return pid;
}

void run_process(uint32 pid)
{
    process *proc=&tab_process[pid];
    if(proc->state!=RUNNABLE)
        return;
    asm volatile("jmp *%0"::"a"(proc->saved_context.eip));
}