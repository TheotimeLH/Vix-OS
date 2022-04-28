#include "video/vga_driver.h"
#include "common/timer.h"
#include "common/descriptor_tables.h"

void test_entry(){
	init_vga(BLACK,WHITE);
	print_string("Hello World\n");
	init_descriptor_tables();
	init_timer(50);
}
