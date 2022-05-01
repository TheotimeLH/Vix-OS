#include "process.h"
#include "../common/kheap.h"

const uint32 n_process=10;
process tab_process[n_process];
extern uint32 placement_adress;
extern page_directory_t* kernel_directory;


void init_process_tab()
{
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
}

uint32 exec(char* filename,Ata_fat_system *afs,Fat_infos* infos,Fat_entry *dir)
{
    Fat_entry entries[10];
    int i;
    while(1)
    {
        int nb_entry=dir->read_entries(entries,10,infos,afs);
        for(i=0;i<nb_entry;i++)
        {
            if(strcmp(entries[i].get_name(),filename))
            {
                print_string("ok");
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
    uint32 pid=load_process(buff);
    tab_process[pid].current_dir=*dir;
		print_string("PID : ");
		print_int(pid);
    return pid;
}


uint32 load_process(uint8* elf,uint32 current_pid)
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
    switch_page_directory(kernel_directory);
    return pid;
}

void run_process(uint32 pid)
{
    process *proc=&tab_process[pid];
    if(proc->state!=RUNNABLE)
        return;
    switch_page_directory(proc->directory);
    asm volatile("jmp *%0"::"a"(proc->saved_context.eip));
    switch_page_directory(kernel_directory);
}
