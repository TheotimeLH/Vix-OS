#include "../../stdlib/stdlib.h"

char* msg="coucou depuis le programme\n";

int main()
{
    print_screen(10,10,'B',RED,GREEN);
    print_screen(11,10,'O',GREEN,WHITE);
    print_screen(12,10,'N',WHITE,MAGENTA);
    print_screen(13,10,'J',MAGENTA,YELLOW);
    print_screen(14,10,'O',YELLOW,CYAN);
    print_screen(15,10,'U',CYAN,BROWN);
    print_screen(16,10,'R',BROWN,RED);
    return 0;	
}
