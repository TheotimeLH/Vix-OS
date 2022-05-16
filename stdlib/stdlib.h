#ifndef STDLIB_H
#define STDLIB_H

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define STDLIB_VERSION
#include "../common/keyboard.h"
#undef STDLIB_VERSION

typedef enum vga_color {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	GREY,
	DARK_GREY,
	BRIGHT_BLUE,
	BRIGHT_GREEN,
	BRIGHT_CYAN,
	BRIGHT_RED,
	BRIGHT_MAGENTA,
	YELLOW,
	WHITE,
} vga_color;

void putchar(char c);
void print_int(int i);
void print_hexa(uint32 x);

uint32 get_ticks();
void write(uint32 file_desc,char *str);
void fwrite(uint32 file_desc,uint8 *data,uint32 size);
uint32 read(uint32 file,uint8* buff,uint32 size);
void print_screen(uint32 x,uint32 y,char car,uint8 fg_color,uint8 bg_color);
keyboard_t get_keyboard();
uint32 open(char* filename);
void exec(char* filename);
uint32 get_pid();
void exit(uint32 i);
uint32 wait(uint32* status);
uint32 get_ppid();
uint32 change_directory(char* name);//1 si c'est ok, 0 sinon
//list_entries : charge les noms des n_entries prochaines entrées
//du répertoire courant dans buff
//buff doit avoir une taille 10*n_entries :
//le nom prend au plus 9 caractères en comptant '\0',
//et le 10eme caractère est 'd' si c'est un répertoire,
//et 'f' si c'est un fichier
//"cd ." permet de réinitialiser le répertoire courant pour
//revenir à la première entrée
uint32 list_entries(char* buff,uint32 n_entries);
uint32 make_directory(char* name);

uint32 strlen(char* str);

#endif
