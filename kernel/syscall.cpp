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

static void syscall(registers_t regs)
{
    //rétablir le contexte du noyau ??
    uint32* eax=(uint32*)(regs.esp-4);
    keyboard_t key;
    bool ok;
    uint32 i;
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
            return;
        }
        current_proc->opened_files[i].type=FAT_ENTRY;
        
        current_proc->opened_files[i].entry=
            open_file((char*)regs.edi,afs,infos,&current_proc->current_dir,&ok);
        current_proc->opened_files[i].cursor=0;
        if(!ok)
        {
            if(!current_proc->current_dir.add_entry((char*)regs.edi,false,
                &current_proc->opened_files[i].entry,infos,afs))
            {
                *eax=uint32(-1);
                return;
            }
        }
        *eax=i;
        break;
    default:
        break;
    }
}

void init_syscalls(Ata_fat_system* syst,Fat_infos *fi)
{
    afs=syst;
    infos=fi;
    register_interrupt_handler(0x42,&syscall);
}