#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main()
{
    change_directory("DIR");
    uint32 f2=open("FILE2");
    change_directory("..");
    uint32 f1=open("FILE");
    char data[101];
    uint32 size=read(f2,data,100);
    data[size]=0;
    write(0,data);
    write(0,"\n");
    size=read(f1,data,100);
    data[size]=0;
    write(0,data);

    while(1);

    return 0;	
}