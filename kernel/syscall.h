#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "../common/isr.h"
#include "../fat_driver/fat_driver.h"

void init_syscalls(Ata_fat_system* syst,Fat_infos *fi);

#endif
