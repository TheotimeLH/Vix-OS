#include "syscall.h"
#include "../common/timer.h"
#include "process.h"
#include "../common/keyboard.h"
#include "../fat_driver/fat_driver.h"

Ata_fat_system *afs=0;
Fat_infos *infos=0;
extern uint32 current_pid;
extern uint32 n_process;
extern process *current_proc;
extern uint32 kernel_stack;
extern page_directory_t* kernel_directory;

uint32 read(uint32 file,uint8 *buffer,uint32 size)
{
    uint32 read_size=0;
    if(file>=max_opened_files||current_proc->opened_files[file].type!=FAT_ENTRY)
    {
        return 0;
    }
    file_desc *fd=&current_proc->opened_files[file];
    Fat_entry *entry=&current_proc->opened_files[file].entry;
    size=(size>entry->get_size()-fd->cursor)?entry->get_size()-fd->cursor:size;

    if(fd->cursor%infos->byte_per_cluster!=0)
    {
        for(;read_size<size;read_size++)
        {
            if(fd->cursor%infos->byte_per_cluster==0)
                break;
            buffer[read_size]=fd->buff[fd->cursor%infos->byte_per_cluster];
            fd->cursor++;
        }
    }
    size-=read_size;
    uint32 cluster_count=size/infos->byte_per_cluster;
    uint32 temp(0);
    if(cluster_count!=0)
    {
        temp=entry->read_data(&buffer[read_size],cluster_count,infos,afs);
    }
    read_size+=temp;
    size-=temp;
    fd->cursor+=temp;

    if(read_size/infos->byte_per_cluster!=cluster_count||size==0)
    {
        return read_size;
    }

    temp=entry->read_data(fd->buff,1,infos,afs);
    size=(size>temp)?temp:size;
    for(;size>0;size--)
    {
        buffer[read_size]=fd->buff[fd->cursor%infos->byte_per_cluster];
        read_size++;
        fd->cursor++;
    }
    return read_size;
}

uint32 read_entries(char* buff,uint32 n_entries)
{
    Fat_entry fat_buff[n_entries];
    uint32 ret=current_proc->current_dir.read_entries(fat_buff,n_entries,infos,afs);
    for(uint32 i=0;i<ret;i++)
    {
        char* name=fat_buff[i].get_name();
        for(int j=0;j<9;j++)
        {
            buff[i*10+j]=name[j];
        }
        if(fat_buff[i].is_directory())
        {
            buff[i*10+9]='d';
        }
        else
        {
            buff[i*10+9]='f';
        }
    }
}

uint32 global_save_pid;
extern uint32 return_proc;
uint32 global_var,global_var2;
static void syscall(registers_t regs)
{
    uint32 current_pid_save=current_pid;
    current_pid=-1;
    uint32* eax=(uint32*)(regs.esp-4);
    uint32* ebx=(uint32*)(regs.esp-16);
    Fat_entry fat_entry;

    keyboard_t key;
    bool ok;
    uint32 i,j;
    switch (*eax)
    {
    case 0://get_ticks
        *eax=get_ticks();
        break;
    case 1://write
        if(regs.edi==0)
        {
            print_string((char*)regs.esi);
        }
        else if(current_proc->opened_files[regs.edi].type==FAT_ENTRY)
        {
            current_proc->opened_files[regs.edi].entry.write_data((uint8*)regs.esi,regs.ebx,infos,afs);
        }
        break;
    case 2://read
        *eax=read(regs.edi,(uint8*)regs.esi,regs.ebx);
        break;
    case 3://print_screen
        change_color((regs.edi>>4)&0xf,regs.edi&0xf);
        write_char(regs.edi>>24,regs.edi>>16&0xff,regs.edi>>8&0xff);
        break;
    case 4://get_keyboard
        key=keyboard_handler();
        *eax=((uint32)key.type)|((*(uint32*)&key.k)<<8);
        break;
    case 5://open
        for(i=0;i<max_opened_files;i++)
        {
            if(current_proc->opened_files[i].type==CLOSED_FILE)
            {
                break;
            }
        }
        if(i==max_opened_files)
        {
            *eax=uint32(-1);
            break;;
        }
        
        current_proc->opened_files[i].entry=
            open_file((char*)regs.edi,afs,infos,&current_proc->current_dir,&ok);
        current_proc->opened_files[i].cursor=0;
        if(!ok)
        {
            if(!current_proc->current_dir.add_entry((char*)regs.edi,false,
                &current_proc->opened_files[i].entry,infos,afs))
            {
                *eax=uint32(-1);
                break;
            }
        }
        current_proc->opened_files[i].type=FAT_ENTRY;
        *eax=i;
        break;
    case 6://exec
        asm volatile("mov %0,%%esp"::"a"(&kernel_stack));
        global_save_pid=exec((char*)regs.edi,afs,infos,&current_proc->current_dir,current_pid_save);
        if(global_save_pid==(uint32)-1)
        {
            break;
        }
        asm volatile("mov %%ebp,%0":"=r"(current_proc->saved_context.ebp));
        asm volatile("lea -0xC(%%ebp),%0":"=a"(current_proc->saved_context.ebx));
        asm volatile("mov %0,(%%ebp)"::"a"(&return_proc));
        sti();
        run_process(global_save_pid);
        break;
    case 7://get pid
        *eax=current_pid_save;
        break;
    case 8://exit
        global_save_pid=current_pid_save;
        global_var=regs.edi;
        global_var2=regs.esp;
        asm volatile("mov %0,%%esp"::"a"(&kernel_stack));
        switch_page_directory(kernel_directory);
        free_dir(current_proc->directory);
        if(current_proc->ppid==(uint32)-1)
            current_proc->state=FREE;
        else
        {
            current_proc->state=ZOMBIE;
            current_proc->status=global_var;
        }
        current_pid=global_save_pid;
        switch_context(global_var2);
        break;
    case 9://wait
        j=sys_wait(&i,current_pid_save);
        if(j==(uint32)-1)
        {
            if(current_proc->state==WAITING)
            {
                *eax=-2;
            }
            else
                *eax=-1;
        }
        else
        {
            *eax=j;
            *ebx=i;
        }
        break;
    case 0xA://get_ppid
        *eax=current_proc->ppid;
        break;
    case 0xB://cd
        if(strcmp((char*)regs.edi,"."))
        {
            current_proc->current_dir.init_offset();
            break;
        }
        fat_entry=open_dir((char*)regs.edi,afs,infos,&current_proc->current_dir,&ok);
        if(!ok)
        {
            *eax=0;
            break;
        }
        *eax=1;
        current_proc->current_dir=fat_entry;
        break;
    case 0xC://ls
        *eax=read_entries((char*)regs.edi,regs.esi);
        break;
    case 0xD://mkdir
        open_dir((char*)regs.edi,afs,infos,&current_proc->current_dir,&ok);
        if(ok)
        {
            *eax=0;
            break;
        }
        ok=current_proc->current_dir.add_entry((char*)regs.edi,true,&fat_entry,infos,afs);
        *eax=(ok)?1:0;
        break;
    default:
        break;
    }
    current_pid=current_pid_save;
}

void init_syscalls(Ata_fat_system* syst,Fat_infos *fi)
{
    afs=syst;
    infos=fi;
    register_interrupt_handler(0x42,&syscall);
}