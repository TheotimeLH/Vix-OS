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
    uint32 buff_pos=0;
    while(1)
    {
      keyboard_t k=get_keyboard();
      if(k.type==0&&k.k.ch!=0)
      {
        print_screen(buff_pos,0,k.k.ch,WHITE,BLACK);
        buffer[buff_pos]=k.k.ch;
        buff_pos++;
      }
      else if(k.type==1&&k.k.sp==ENTER)
      {
        init_vga();
        buffer[buff_pos]=0;
        exec(buffer);
        int status;
        int pid=wait(&status);
        print_screen(0,0,' ',WHITE,BLACK);
        init_vga();
        write(0,buffer);
        print_int(pid);
        write(0,"\n");
        break;
      }
      else if (k.type==1&&k.k.sp==BACKSPACE&&buff_pos>=1&&buff_pos<100)
      {
        buff_pos--;
        print_screen(buff_pos,0,' ',WHITE,BLACK);
      }
    }
  }
}