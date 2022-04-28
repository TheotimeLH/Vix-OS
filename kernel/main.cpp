#include "../video/vga_driver.h"
#include "../common/descriptor_tables.h"
#include "../common/timer.h"


extern "C" void kernel_main()
{
	init_descriptor_tables();
    init_vga(0x07,0x0);
    print_string("coucou\n");
	init_timer(20);
    

    while(1);
}
