#include "vga_driver.h"
#include "../common/common.h"
#include "../sound/sound_driver.h"

// Index pour le buffer video
uint32 vga_index;
// compteur pour la prochaine ligne
static uint32 next_line_index = 1;
// Couleurs du fond et du premier plan
uint8 g_fore_color = WHITE, g_back_color = BLACK;
// Les chiffres en ascii
uint32 digit_ascii[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color){
	/*
	 * 16 bits de video
	 * 8 premiers bits : 4 pour la couleur du fond, et 4 pour la couleur du premier plan
	 * 8 derniers bits : quel caractere ascii imprimer
	 */
	uint16 ax = 0;
	uint8 ah = 0, al = 0;
	ah = back_color;
	ah <<= 4;
	ah |= fore_color;
	ax = ah;
	ax <<= 8;
	al = ch;
	ax |= al;
	return ax;
}

// clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color){
	uint32 i;
	for(i = 0; i < BUFSIZE; i++)
		(*buffer)[i] = vga_entry(NULL,fore_color, back_color);
}

//initialize vga buffer 

void init_vga(uint8 fore_color, uint8 back_color){
//	if(current_mode != TEXT)
//		change_mode(TEXT);
	vga_buffer = (uint16*)VGA_ADDRESS_TEXT;
	clear_vga_buffer(&vga_buffer, fore_color, back_color);
	g_fore_color = fore_color;
	g_back_color = back_color;
}


// Printing 

void print_new_line(){
	if(next_line_index >= TEXT_HEIGHT){
		// On revient au début de l'écran et on clear tout
		next_line_index = 0;
		clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
	}
	vga_index = TEXT_WIDTH * next_line_index;
	next_line_index++;
}

void print_char(char ch){
	if(ch == '\n')
		print_new_line();
	else
		vga_buffer[vga_index++] = vga_entry(ch, g_fore_color, g_back_color);
}

void print_string(char *str){
	int i = 0;
	while(str[i]){
		print_char(str[i]);
		i++;
	}
}

void print_int(int num){
	char str[digit_count(num) + 1];
	itoa(num, str);
	print_string(str);
}

void write_char(int x, int y, char c){
	vga_buffer[TEXT_WIDTH*y + x] = vga_entry(c, g_fore_color, g_back_color);

}
// Color manager 

void change_color(uint8 fore_color, uint8 back_color){
	g_fore_color = fore_color;
	g_back_color = back_color;
}

// test entry - TO BE DELETED LATER ON -
void test_entry()
{
	init_vga(RED,BLUE);
	print_new_line();
	print_string("Bienvenue sur le Vix-OS");
	print_new_line();
	print_string("Test");
	print_new_line();
	print_int(12345);
	print_char(0x3d);
	print_new_line();
	print_string("Vim est le meilleur logiciel de tout les temps\n");
	change_color(RED,WHITE);
	print_string("EMACS est un éditeur pour hérétique\n");
	print_string("    ");
	beep();
	print_string("J'ai beepé");
	/*
	change_mode(GRAPHIC);
	int d = 0;
	while(1){
		draw_pixel(d%GRAPHIC_HEIGHT,d%GRAPHIC_WIDTH,9);
	}	
	change_mode(TEXT);
	*/
//	draw_rectangle(10,10,0,0,'@');
//	draw_rectangle_color(20,20,10,10, '@', BLACK,WHITE);
//	init_vga_graphic(WHITE,BLACK);

}
