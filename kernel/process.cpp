#include "process.h"
#include "../common/kheap.h"

const uint32 n_process=10;
process tab_process[n_process];
extern uint32 placement_adress;
extern page_directory_t* kernel_directory;
extern page_directory_t* current_directory;
uint32 current_pid;
extern Fat_infos fi;
process* current_proc;

void init_process_tab()
{
    for(int i=0;i<n_process;i++)
    {
        for(uint32 j=0;j<max_opened_files;j++)
        {
            tab_process[i].opened_files[j].buff=(uint8*)kmalloc(fi.byte_per_cluster);
        }
    }
    for(int i=0;i<n_process;i++)
    {
        tab_process[i].state=FREE;
        tab_process[i].directory=(page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	    memset(tab_process[i].directory, 0, sizeof(page_directory_t));
        for(uint32 pn=0;pn<1024;pn++)
        {
            get_page(pn<<22,1,tab_process[i].directory);
        }
    }
    current_pid=-1;
}

uint32 exec(char* filename,Ata_fat_system *afs,Fat_infos* infos,Fat_entry *dir,uint32 ppid)
{
    exec(filename,afs,infos,dir,ppid,"");
}

uint32 exec(char* filename,Ata_fat_system *afs,Fat_infos* infos,Fat_entry *dir,uint32 ppid,char* arg)
{

    Fat_entry entries[10];
    int i;
    dir->init_offset();
    while(1)
    {
        int nb_entry=dir->read_entries(entries,10,infos,afs);
        for(i=0;i<nb_entry;i++)
        {
            if(strcmp(entries[i].get_name(),filename))
            {
                break;
            }
        }
        if(i<nb_entry)
            break;
        if(nb_entry==0)
            return uint32(-1);
    }
    
    uint32 size=entries[i].get_size();
    uint32 cluster_count=(size+infos->byte_per_cluster-1)/infos->byte_per_cluster;
    uint8 buff[cluster_count*infos->byte_per_cluster];
    entries[i].read_data(buff,cluster_count,infos,afs);
    uint32 pid=load_process(buff,ppid,arg);
    if(pid==-1)
        return pid;
    tab_process[pid].current_dir=*dir;
    tab_process[pid].opened_files[STDIN].type=STDIN;
    tab_process[pid].opened_files[STDOUT].type=STDOUT;
    for(int i=2;i<max_opened_files;i++)
    {
        tab_process[pid].opened_files[i].type=CLOSED_FILE;
    }
    return pid;
}

uint32 load_process(uint8* elf,uint32 ppid,char* arg)
{
    uint32 len=strlen(arg);
    char kernel_stack_arg[len+1];
    for(uint32 i=0;i<len;i++)
    {
        kernel_stack_arg[i]=arg[i];
    }
    kernel_stack_arg[len]=0;

    uint32 pid;
    for(pid=0;pid<n_process;pid++)
    {
        if(tab_process[pid].state==FREE)
            break;
    }
    if(pid==n_process)
    {
        for(pid=0;pid<n_process;pid++)
        {
            if(tab_process[pid].state==ZOMBIE)
            {
                tab_process[pid].state=FREE;
                break;
            }
        }
        if(pid==n_process)
            return (uint32)-1;
    }
    process* proc=&tab_process[pid];

    //on map le kernel
	for(uint32 i=0;i<placement_adress;i+=0x1000)
	{
		alloc_frame(get_page(i,1,proc->directory),1,1,i,true);
	}
    
    switch_page_directory(proc->directory);

    char elf_sign[4]="ELF";
    bool is_elf=true;
    for(int i=0;i<3;i++)
    {
        is_elf=is_elf&&(elf_sign[i]==elf[i+1]);
    }
    if(!(elf[0]==0x7F&&is_elf))
    {
        print_string("erreur de format\n");
        return -1;
    }
    
    proc->pg_entry=get_uint32(elf,0x18);
    proc->ppid=ppid;
    proc->saved_context.ds=0x10;
    proc->saved_context.cs=0x10;
    proc->saved_context.ss=0x10;
    proc->saved_context.eip=proc->pg_entry;
    
    uint32 pg_header=get_uint32(elf,0x1C);
    uint16 n_entry=get_uint16(elf,0x2C);
    uint32 max_addr(0);
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
        if((uint32)seg_in_mem+size_in_mem>=max_addr)
            max_addr=(uint32)seg_in_mem+size_in_mem;
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
    if(max_addr!=0)
    {
        len=strlen(kernel_stack_arg);
        for(uint32 i=0;i<len;i++)
        {
            ((uint8*)max_addr)[i]=kernel_stack_arg[i];
        }
        ((uint8*)max_addr)[len]=0;
        proc->saved_context.edi=max_addr;
    }
    proc->state=RUNNABLE;
    return pid;
}

void run_process(uint32 pid)
{
    if(pid>=n_process)
        return;
    process *proc=&tab_process[pid];
    if(proc->state!=RUNNABLE)
        return;
    switch_page_directory(proc->directory);
    current_pid=pid;
    current_proc=proc;
    asm volatile("mov %0,%%edi"::"a"(proc->saved_context.edi));
    asm volatile("jmp *%0"::"a"(proc->saved_context.eip));
    switch_page_directory(kernel_directory);
}

uint32 sys_wait(uint32* status,uint32 pid)
{
    bool has_son=false;
    for(int i=0;i<n_process;i++)
    {
        if((tab_process[i].state==ZOMBIE||tab_process[i].state==RUNNABLE)&&tab_process[i].ppid==pid)
        {
            has_son=true;
        }
        if(tab_process[i].state==ZOMBIE&&tab_process[i].ppid==pid)
        {
            *status=tab_process[i].status;
            tab_process[i].state=FREE;
            tab_process[pid].state=RUNNABLE;
            return i;
        }
    }
    if(!has_son)
    {
        tab_process[pid].state=RUNNABLE;
        return -1;
    }

    tab_process[pid].state=WAITING;
    return -1;
}

uint32 trans_ebp;
extern uint32 kernel_stack;
uint32 global_pid;
extern "C" void switch_context(uint32 curr_esp)
{
    current_proc->saved_context.esp=curr_esp;
    if(current_pid==-1)
        return;

    asm volatile("mov %%ebp,%0":"=r"(trans_ebp));
    current_proc->saved_context.ebp=trans_ebp;
    asm volatile("mov %%ebx,%0":"=r"(trans_ebp));
    current_proc->saved_context.ebx=trans_ebp;

    do
    {
        current_pid=(current_pid+1)%n_process;
        if(tab_process[current_pid].state==ZOMBIE)
        {
            for(uint32 i=0;i<n_process;i++)
            {
                if((tab_process[i].state!=FREE)&&tab_process[i].ppid==current_pid)
                {
                    tab_process[i].ppid=-1;                    
                }
            }
            if(tab_process[current_pid].ppid==-1)
            {
                tab_process[current_pid].state=FREE;
                print_string("processus sans pere termine, avec le status : ");
                print_int(tab_process[current_pid].status);
                print_new_line();
            }
        }
        if(tab_process[current_pid].state==WAITING)
        {
            global_pid=sys_wait(&tab_process[current_pid].status,current_pid);
            if(global_pid==-1&&tab_process[current_pid].state==RUNNABLE)
            {
                //pas de fils, return -1
                current_proc=&tab_process[current_pid];
                asm volatile("mov %0,%%esp"::"a"(&kernel_stack));
                switch_page_directory(current_proc->directory);
                *(uint32*)(tab_process[current_pid].saved_context.esp-4)=-1;//eax
                trans_ebp=current_proc->saved_context.ebp;
                asm volatile("mov %0,%%ebp"::"a"(trans_ebp));
                trans_ebp=current_proc->saved_context.ebx;
                asm volatile("mov %0,%%ebx"::"a"(trans_ebp));
                return;
            }
            else if(global_pid!=-1)
            {
                //un fils est fini, process runnable
                current_proc=&tab_process[current_pid];
                asm volatile("mov %0,%%esp"::"a"(&kernel_stack));
                switch_page_directory(current_proc->directory);
                *(uint32*)(tab_process[current_pid].saved_context.esp-4)=global_pid;
                *(uint32*)(tab_process[current_pid].saved_context.esp-16)=current_proc->status;

                trans_ebp=current_proc->saved_context.ebp;
                asm volatile("mov %0,%%ebp"::"a"(trans_ebp));
                trans_ebp=current_proc->saved_context.ebx;
                asm volatile("mov %0,%%ebx"::"a"(trans_ebp));
                return;
            }
        }
    }while(tab_process[current_pid].state!=RUNNABLE);

    current_proc=&tab_process[current_pid];
    asm volatile("mov %0,%%esp"::"a"(&kernel_stack));
    switch_page_directory(current_proc->directory);
    trans_ebp=current_proc->saved_context.ebp;
    asm volatile("mov %0,%%ebp"::"a"(trans_ebp));
    trans_ebp=current_proc->saved_context.ebx;
    asm volatile("mov %0,%%ebx"::"a"(trans_ebp));
}