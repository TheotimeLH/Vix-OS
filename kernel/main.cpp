#include "../video/vga_driver.h"
#include "../common/descriptor_tables.h"
#include "../common/timer.h"
#include "../fat_driver/fat_driver.h"
#include "../common/paging.h"
#include "process.h"
#include "syscall.h"
#include "../common/keyboard.h"
#include "multiboot.h"

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
        if(strcmp(entries[i].get_name(),"MVIM"))
				{
						print_string("Programme MVIM charge");
            break;
				}
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

uint32 memory_detection(multiboot_info_t *mbd,uint32 magic);//return memory end

extern "C" void kernel_main(multiboot_info_t* mbd,uint32 magic)
{
    init_descriptor_tables();
    //init_paging();
    init_process_tab();
    init_vga(0x07,0x0);
    init_timer(1000);
    init_syscalls();
	init_keyboard();
    memory_detection(mbd,magic);

//    charger_prog();
    while (1);
    


     while(1){
			 keyboard_t k = keyboard_handler();
			 if (!k.type && k.k.ch != 0){
				 print_char(k.k.ch);
			 }
			 else{
				 if(k.k.sp == SPACE){
					 print_char(' ');
				 }
			 }
		 }
}


uint32 memory_detection(multiboot_info_t *mbd,uint32 magic)
{
    if(magic!=MULTIBOOT_BOOTLOADER_MAGIC||!(mbd->flags>>6&0x1))
    {
        PANIC("memory map mal chargee par grub\n");
    }

    for(int i=0; i < mbd->mmap_length;i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE&&mmmt->addr==0x100000)
        {
            return mmmt->len;
        }
    }

    PANIC("pas de memoire trouvee\n");
}
