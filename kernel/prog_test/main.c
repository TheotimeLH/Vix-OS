#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main(char* arg)
{
    write(0,arg);
    write(0,"coucou\n");
    if(get_pid()==0)
    {
        execa("PROG","ceci est un autre argument\n");
        // int status;
        // int pid=wait(&status);
        // if(pid!=-1)
            // print_int(status);
        // write(0,"\n");
    }
    else
    {
        pause();
        return 42;
    }

    return 69;	
}