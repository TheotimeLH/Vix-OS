#include "../video/vga_driver.h"
#include "../common/descriptor_tables.h"
#include "../common/timer.h"
#include "../fat_driver/fat_driver.h"
#include "../common/paging.h" 
#include "process.h"
#include "syscall.h"
#include "../common/keyboard.h"
#include "multiboot.h"

void init_disk(Fat_infos *fi,Ata_fat_system *afs);

uint32 memory_detection(multiboot_info_t *mbd,uint32 magic);//return memory size


extern "C" void kernel_main(multiboot_info_t* mbd,uint32 magic)
{
    init_descriptor_tables();
    init_vga(0x07,0x0);
    init_process_tab();
    init_timer(1000);
    init_syscalls();
		init_keyboard();
    init_paging(memory_detection(mbd,magic));
    Fat_infos fi;
    Ata_fat_system afs(hda);
    init_disk(&fi,&afs);
    uint32 pid=exec("PROG",&afs,&fi,&(fi.root_fat_entry));
    if(pid!=uint32(-1))
    {
        run_process(pid);
    }

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

void init_disk(Fat_infos *fi,Ata_fat_system *afs)
{
    if(!afs->is_ready())
    {
        print_string("erreur disque\n");
        while(1);
    }
    *fi=fat_init(afs);
    if(fi->fat_type!=12&&fi->fat_type!=16&&fi->fat_type!=32)
    {
        print_string("erreur systeme de fichier\n");
        while(1);
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
