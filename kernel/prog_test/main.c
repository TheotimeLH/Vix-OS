#include "../../stdlib/stdlib.h"

int main()
{
    write(0,"programme lance\n");
    char buff[100];
    uint32 file=open("FILE");
    uint32 size=read(file,buff,99);
    buff[size+1]=0;
    write(0,buff);

    write(0,"ok");

    while(1);

    return 0;	
}
