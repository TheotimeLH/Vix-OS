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
        if(buffer[0]=='l'&&buffer[1]=='s'&&buffer[2]==0)
        {
          change_directory(".");
          int n_entries=list_entries(buffer,10);
          for(int i=0;i<n_entries;i++)
          {
            write(0,buffer+i*10);
            write(0,"\n");
          }
          do
          {
            k=get_keyboard();
          } while (k.type!=1||k.k.sp!=ENTER);
          init_vga();
          change_directory(".");
          break;
        }
        else if(buffer[0]==':'&&buffer[1]=='q'&&buffer[2]==0)
        {
          return 1515;
        }
        else if(buffer[0]==':'&&buffer[1]=='e'&&buffer[2]==0)
        {
          uint32 f=open("FILE");
          uint32 size=read(f,buffer,99);
          buffer[size]=0;
          write(0,buffer);
          do
          {
            k=get_keyboard();
          } while (k.type!=1||k.k.sp!=ENTER);
          init_vga();
          change_directory(".");
          break;
        }
        exec(buffer);
        int status;
        int pid=wait(&status);
        print_screen(0,0,' ',WHITE,BLACK);
        init_vga();
        print_int(status);
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