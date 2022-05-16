#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main()
{
    char buff[100];
    uint32 size=list_entries(buff,10);
    for(int i=0;i<size;i++)
    {
        write(0,&buff[i*10]);
        if(buff[i*10+9]=='d')
        {
            write(0," d\n");
        }
        else if(buff[i*10+9]=='f')
        {
            write(0," f\n");
        }
        else
        {
            write(0," e\n");
        }
    }

    while(1);

    return 0;	
}