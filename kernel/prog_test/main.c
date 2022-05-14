#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main()
{
    write(0,"[");
    print_int(get_pid());
    write(0,"] ");
    write(0,"programme lance, pere : ");
    print_int(get_ppid());
    write(0,"\n");

    if(get_pid()==0)
    {
        exec("PROG");
        pause();
        exec("PROG");
    }
    else
    {
        exit(get_pid());
    }

    pause();

    uint32 pid;
    do
    {
        uint32 status;
        pid=wait(&status);
        if(pid==-1)
        {
            write(0,"[");
            print_int(get_pid());
            write(0,"] ");
            write(0,"pas de fils");
        }
        else
        {
            write(0,"[");
            print_int(get_pid());
            write(0,"] ");
            write(0,"fils termine : ");
            print_int(pid);
            write(0," status : ");
            print_int(status);
            write(0,"\n");
        }
    }while(pid!=-1);

    while(1);

    return 0;	
}