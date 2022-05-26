#include "../stdlib/stdlib.h"

int x,y ;
char bufs[22][82] ;

void empty_line(int n)
{
	for (int i=0 ; i<82 ; i++) bufs[n][i] = '\0' ;
	bufs[n][0] = '$' ; bufs[n][1] = ' ' ;
}

void print_lines(int x)
{
	for (int i=0 ; i<22 ; i++)
		for (int j=0 ; j<81 ; j++)
			print_screen(j, i-x-1+(i<x+1?22:0), bufs[i][j], WHITE, BLACK) ;
}

int ls()
{
	int n = list_entries(bufs[x]+2, 8) ;
	for (int i=0 ; i<n ; i++)
	{
		int j ;
		for (j=0 ; bufs[x][j+10*i+2] ; j++) ;
		for ( ; j<10 ; j++) bufs[x][j+10*i+2] = '\0' ;
	}
	return n ;
}

void rec_rm(char* dossier)
{
	change_directory(dossier) ;
	char buf[10] ;
	while (list_entries(buf, 1))
		if (strCmp(buf, ".", 1)==0 || strCmp(buf, "..", 2)==0) ;
		else if (buf[9]=='d') rec_rm(buf) ;
		else remove_entry(buf) ;
	change_directory("..") ;
	remove_entry(dossier) ;
}

void eval()
{
	char line[80] ;
	strcpy(line, bufs[x]+2) ;
	if (strCmp(line, "ls", 2)==0)
	{
		change_directory(".") ;
		do empty_line(x = ++x % 22) ;
		while (ls()) ;
		change_directory(".") ;
		x-- ;
	}
	else if (strCmp(line, "cd ", 2)==0)	change_directory(line+3) ;

	else if (strCmp(line, "mkdir ", 5)==0) make_directory(line+6) ;

	else if (strCmp(line, "rm ", 2)==0)
	{
		char buf[10] ;
		while (list_entries(buf, 1))
			if (strCmp(line+3, buf, 8)==0) {
				if (buf[9]=='d') rec_rm(buf) ;
				else remove_entry(buf) ;
				break ;	}
		change_directory(".") ;
	}
	else if (strCmp(line, "cat ", 3)==0)
	{
		uint32 f = open(line+4) ;
		do empty_line(x = ++x % 22) ;
		while (read(f, bufs[x], 81)) ;
		change_directory(".") ;
		x-- ;
	}
	else
	{
		int i ;
		for (i=0 ; line[i] && line[i]!=' ' ; i++) ;
		if (line[i]==' ')	{
			line[i] = '\0' ;
			execa(line, line+i+1) ;
			line[i] = ' ' ; }
		else exec(line) ;
		int status ;
		int pid = wait(&status) ;
	}
}

int main()
{
	int b=0 ;
	x=0 ;	y=2 ;
	for (int i=0 ; i<22 ; i++) empty_line(i) ;
	print_lines(x) ;
  while(1)
  {

		print_screen(y, 21, b++&(1<<18)?'_':' ', WHITE, BLACK) ;
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
		else if (k.k.sp==BACKSPACE && y>2)
		{
			for (int i=--y ; i<81 ; i++) {
				bufs[x][i] = (i==80) ? '\0' : bufs[x][i+1] ;
				print_screen(i, 21, bufs[x][i], WHITE, BLACK) ;
			}
		}
		else if (k.k.sp==ENTER)
		{
			if(strCmp(bufs[x]+2, "exit", 4)==0) return 1515 ;
			eval() ;
			x = ++x % 22 ; y=2 ;
			empty_line(x) ;
			print_lines(x) ;
		}
  }
}

