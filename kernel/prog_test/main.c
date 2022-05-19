#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main(char* arg)
{
<<<<<<< HEAD
		while(1)
		{
			keyboard_t k=get_keyboard();
			if(k.type==1)
				write(0,"\n");
			else
			{
				char str[2];
				str[1]=0;
				str[0]=k.k.ch;
				write(0,str);
			}
		}


    return 69;	
}
=======
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
>>>>>>> 15d02e05ba57df665d76db25cb8c1d3123059fdb
