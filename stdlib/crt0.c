#include "stdlib.h"

extern int main(char* arg);

void crt0_main()
{
  char* arg;
  asm volatile("mov %%edi,%0":"=a"(arg));

  uint32 len=strlen(arg);
  char my_stack_arg[len];
  for(uint32 i=0;i<len;i++)
  {
    my_stack_arg[i]=arg[i];
  }
  my_stack_arg[len]=0;

  exit(main(my_stack_arg));

  while(1);
}