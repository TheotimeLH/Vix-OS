#include "../video/vga_driver.h"
#include "../common/descriptor_tables.h"
#include "../common/timer.h"
#include "../fat_driver/fat_driver.h"
#include "../common/paging.h"
#include "process.h"
#include "syscall.h"

void charger_prog()
{
    Ata_fat_system afs(hda);
    if(!afs.is_ready())
    {
        print_string("erreur disque\n");
        while(1);
    }

    Fat_infos infos=fat_init(&afs);
    Fat_entry entries[10];
    int nb_entry=infos.root_fat_entry.read_entries(entries,10,&infos,&afs);
    int i;
    for(i=0;i<nb_entry;i++)
    {
        if(strcmp(entries[i].get_name(),"PROG"))
            break;
    }
    if(i==nb_entry)
    {
        print_string("erreur prog non trouve\n");
        while(1);
    }
    
    uint32 size=entries[i].get_size();
    uint32 cluster_count=(size+infos.byte_per_cluster-1)/infos.byte_per_cluster;
    uint8 buff[cluster_count*infos.byte_per_cluster];
    entries[i].read_data(buff,cluster_count,&infos,&afs);
    run_process(load_process(buff));
}

extern "C" void kernel_main()
{
    init_descriptor_tables();
		//init_paging();
    init_process_tab();
    init_vga(0x07,0x0);
    init_timer(1000);
    init_syscalls();
    charger_prog();


//		uint32 *ptr = (uint32*)0xA0000000;
//		uint32 do_page_fault = *ptr;
//		return ;
    while(1);
}
