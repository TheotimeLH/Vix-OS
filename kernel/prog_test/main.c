#include "../../stdlib/stdlib.h"

int main()
{
    write(0,"programme lance\n");

    uint32 pid=1;
    if(get_pid()==0)
    {
        pid=0;
        write(0,"pid null\n");
        exec("PROG");
    }


    uint32 ticks=0;
    while(1)
    {
        if(1)//get_ticks()>=ticks+1000)
        {
            if(pid==0)
                write(0,"prog0\n");
            else
                write(0,"prog1\n");
       //     ticks=get_ticks();
        }
    }

    return 0;	
}
