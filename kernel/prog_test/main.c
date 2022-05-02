#include "../../stdlib/stdlib.h"

int main()
{
    write(0,"programme lance\n");
    uint32 file=open("FILE");

    char buff[100]="j'écris dans le fichier test depuis un\nprogramme utilisateur !!!";
    write(file,buff);

    uint32 size=read(file,buff,99);
    buff[size+1]=0;
    write(0,buff);

    write(0,"\nok");

    while(1);

    return 0;	
}
