#include "../../stdlib/stdlib.h"

void pause()
{
    uint32 ticks=get_ticks();
    uint32 disp=ticks;
    while(get_ticks()<=ticks+500);
}

int main()
{
    make_directory("TEST");
    char buff[100];
    change_directory(".");
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
    change_directory("TEST");

    uint32 f=open("FTEST");
    char data[101]="coucou";
    write(f,data);

    change_directory("..");
    change_directory("TEST");
    uint32 f2=open("FTEST");
    size=read(f,data,100);

    data[size]='\n';
    data[size+1]=0;
    write(0,data);

    change_directory(".");
    size=list_entries(buff,10);
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