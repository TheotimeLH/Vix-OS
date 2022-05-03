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
    uint32 pid=load_process(buff,ppid);
    tab_process[pid].current_dir=*dir;
    tab_process[pid].opened_files[STDIN].type=STDIN;
    tab_process[pid].opened_files[STDOUT].type=STDOUT;
    for(int i=2;i<max_opened_files;i++)
    {
        tab_process[pid].opened_files[i].type=CLOSED_FILE;
    }
    return pid;
}

uint32 load_process(uint8* elf,uint32 ppid)
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
        return n_process;
    }
    
    proc->pg_entry=get_uint32(elf,0x18);
    proc->ppid=ppid;
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
    switch_page_directory(current_directory);
    return pid;
}

void run_process(uint32 pid)
{
    bool debug=current_pid!=-1;
    process *proc=&tab_process[pid];
    if(proc->state!=RUNNABLE)
        return;
    current_pid=pid;
    current_proc=proc;
    switch_page_directory(proc->directory);
    asm volatile("jmp *%0"::"a"(proc->saved_context.eip));
    switch_page_directory(kernel_directory);
}


uint32 trans_ebp;
extern uint32 kernel_stack;
extern "C" void switch_context()
{
    if(current_pid==-1)
        return;
    do
    {
        current_pid=(current_pid+1)%n_process;
    }while(tab_process[current_pid].state!=RUNNABLE);

    asm volatile("mov %%ebp,%0":"=r"(trans_ebp));
    current_proc->saved_context.ebp=trans_ebp;
    asm volatile("mov %%ebx,%0":"=r"(trans_ebp));
    current_proc->saved_context.ebx=trans_ebp;

    current_proc=&tab_process[current_pid];
    asm volatile("mov %0,%%esp"::"a"(&kernel_stack));
    switch_page_directory(current_proc->directory);
    trans_ebp=current_proc->saved_context.ebp;
    asm volatile("mov %0,%%ebp"::"a"(trans_ebp));
    trans_ebp=current_proc->saved_context.ebx;
    asm volatile("mov %0,%%ebx"::"a"(trans_ebp));
}