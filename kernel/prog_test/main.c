#include "../../stdlib/stdlib.h"

char* msg="coucou depuis le programme\n";

int main()
{
    int tick=0;
    int old_tick=0;
    int changement=0;
    while(1)
    {
        old_tick=tick;
        tick=get_ticks();
        changement=tick!=old_tick;
        if(tick%1000==0&&changement)
        {
            write(0,msg);
        }
    }
    return 0;	
}
