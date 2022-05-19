#include "../stdlib/stdlib.h"

void init_vga()
{
  for(int i=0;i<50;i++)
  {
    write(0,"\n");
  }
}

int main()
{
  char buffer[100];
  init_vga();
  while(1)
  {
    int buff_pos=0;
    while(1)
    {
      keyboard_t k=get_keyboard();
      if(k.type==0&&k.k.ch!=0)
      {
        char str[2];
        str[0]=k.k.ch;
        str[1]=0;
        write(0,str);
        buffer[buff_pos]=k.k.ch;
        buff_pos++;
      }
      else if(k.type==1&&k.k.sp==ENTER)
      {
        exec(buffer);
        int status;
        wait(&status);
        init_vga();
        print_int(status);
        write(0,"\n");
        break;
      }
    }
  }
}