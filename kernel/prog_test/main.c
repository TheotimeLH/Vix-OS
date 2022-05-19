#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main(char* arg)
{
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
