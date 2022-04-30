#include "../../stdlib/stdlib.h"

char* msg="coucou depuis le programme\n";

int main()
{
     while(1){
			 keyboard_t k = get_keyboard();
			 if (!k.type && k.k.ch != 0){
				 print_screen(0,0,k.k.ch,WHITE,BLACK);
			 }
			 else{
				 if(k.k.sp == SPACE){
                    print_screen(0,0,' ',WHITE,BLACK);
				 }
			 }
		 }
    // print_screen(10,10,'B',RED,GREEN);
    // print_screen(11,10,'O',GREEN,WHITE);
    // print_screen(12,10,'N',WHITE,MAGENTA);
    // print_screen(13,10,'J',MAGENTA,YELLOW);
    // print_screen(14,10,'O',YELLOW,CYAN);
    // print_screen(15,10,'U',CYAN,BROWN);
    // print_screen(16,10,'R',BROWN,RED);
    return 0;	
}
