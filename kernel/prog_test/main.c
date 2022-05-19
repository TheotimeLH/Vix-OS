#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main(char* arg)
{
    if(get_pid()==0)
    {
        exec("PROG");
    }

    while(1)
    {
        write(0,"prog ");
        print_int(get_pid());
        write(0,"\n");
    }

    return 0;	
}