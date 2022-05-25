#include "../stdlib/stdlib.h"

void init_vga()
{
  for(int i=0 ; i<26 ; i++) write(0,"\n") ;
}

void empty_line(char line[81])
{
	for (int i=0 ; i<81 ; i++) line[i] = '\0' ;
}

void print_lines(char bufs[22][81], int x)
{
	for (int i=0 ; i<22 ; i++)
		for (int j=0 ; j<81 ; j++)
			print_screen(j, (x+1-i)%22, bufs[i][j], WHITE, BLACK) ;
}

void eval(char line[81])
{
	if (strCmp(line, "ls")==0)
	{
		char buf[100] ;
		change_directory(".") ;
		int n_entries = list_entries(buf, 10) ;
		for(int i=0 ; i<n_entries ; i++) {
			write(0, buf+i*10) ;
			write(0, "\n") ; }
		keyboard_t k ;
		do k = get_keyboard() ;
		while (k.type!=1 || k.k.sp!=ENTER) ;
		init_vga() ;
		change_directory(".") ;
	}
	else if(strCmp(line,":e")==0)
	{
		char buf[100] ;
		uint32 f = open("FILE") ;
		uint32 size = read(f, buf, 99) ;
		buf[size] = '\0' ;
		write(0, buf) ;
		keyboard_t k ;
		do k=get_keyboard() ;
		while (k.type!=1 || k.k.sp!=ENTER) ;
		init_vga();
		change_directory(".");
	}
	else
	{
		exec(line) ;
		int status ;
		int pid = wait(&status) ;
		print_screen(0, 0, ' ', WHITE, BLACK) ;
		init_vga() ;
		print_int(status) ;
		write(0,"\n") ;
	}
}

int main()
{
  char bufs[22][81] ;
	for (int i=0 ; i<22 ; i++) empty_line(bufs[i]) ;
  init_vga() ;
	int x=0, y=0 ;
  while(1)
  {
		keyboard_t k = get_keyboard() ;
		if (k.type==0)
		{
			if (y<81 && k.k.ch!='\0')	{
				print_screen(y, 21, k.k.ch, WHITE, BLACK) ;
				bufs[x][y++] = k.k.ch ;
			}
		}
		else if (k.k.sp==SPACE && y<81)
		{
			print_screen(y, 21, ' ', WHITE, BLACK) ;
			bufs[x][y++] = ' ' ;
		}
		else if (k.k.sp==BACKSPACE && y>0)
		{
			for (int i=--y ; i<81 ; i++) {
				bufs[x][i] = (i==80) ? '\0' : bufs[x][i+1] ;
				print_screen(i, 21, bufs[x][i], WHITE, BLACK) ;
			}
		}
		else if (k.k.sp==ENTER)
		{
			init_vga() ;
			if(strCmp(bufs[x], ":q")==0) return 1515 ;
			eval(bufs[x]) ;
			y=0 ;
			x = ++x % 22 ;
			empty_line(bufs[x]) ;
			print_lines(bufs, x) ;
		}
  }
}

