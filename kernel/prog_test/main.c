#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main()
{
    write(0,"coucou");
    if(get_pid()==0)
    {
        exec("PROG");
        int status;
        int pid=wait(&status);
        if(pid!=-1)
            print_int(status);
    }
    else
    {
        exit(42);
    }
    while(1);

    return 0;	
}