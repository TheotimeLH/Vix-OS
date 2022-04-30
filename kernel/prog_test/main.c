char* msg="coucou depuis le programme\n";

int main()
{
    int tick=0;
    int old_tick=0;
    int changement=0;
    while(1)
    {
        old_tick=tick;
        asm volatile("mov $0,%eax");
        asm volatile("int $0x42");
        asm volatile("mov %%eax,%0"::"m"(tick));
        changement=tick!=old_tick;
        if(tick%1000==0&&changement)
        {
            asm volatile("mov %0,%%esi":"=m"(msg));
            asm volatile("mov $1,%eax");
            asm volatile("mov $0,%edi");
            asm volatile("int $0x42");
        }
    }
    return 0;	
}
