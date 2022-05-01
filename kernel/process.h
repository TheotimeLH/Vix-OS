#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "../common/common.h"
#include "../common/isr.h"
#include "../fat_driver/fat_driver.h"
#include "../common/paging.h"

#define get_uint32(buff,addr) *(uint32*)(&buff[addr])
#define get_uint16(buff,addr) *(uint16*)(&buff[addr])

typedef enum proc_state
{
    WAITING,RUNNABLE,ZOMBIE,FREE
} proc_state;

const uint32 max_path_length=100;
const uint32 max_opened_files=10;

typedef enum file_type
{
    STDIN,STDOUT,FAT_ENTRY
} file_type;

typedef struct file_desc
{
    file_type type;
    Fat_entry entry;
} file_desc;

typedef struct process
{
    uint32 ppid;
    proc_state state;
    registers_t saved_context;
    uint32 pg_entry;
    page_directory_t *directory;
    char path[max_path_length];
    Fat_entry current_dir;
    file_desc opened_files[max_opened_files];
} process;

void init_process_tab();
uint32 exec(char* filename,Ata_fat_system *afs,Fat_infos* infos,Fat_entry *dir);
uint32 load_process(uint8* elf,uint32 current_pid=0);
void run_process(uint32 pid);

#endif