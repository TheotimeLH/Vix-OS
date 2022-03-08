#include "vga.h"

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
	vga_buffer = (uint16*)VGA_ADDRESS_CHAR;
	clear_vga_buffer(&vga_buffer, fore_color, back_color);
	g_fore_color = fore_color;
	g_back_color = back_color;
}

// Misc 
uint32 digit_count(int num){
	uint32 count = 0;
	if(num == 0)
		return 1;
	else{
		while(num){
			count ++;
			num /= 10;
		}
		return count;
	}
}

uint32 strlen(const char* str){
	uint32 length = 0;
	while(str[length++]);
	return length;
}

void itoa(int num, char* number){
	int dg = digit_count(num);
	int i = dg -1;
	if(num == 0 && dg == 1){
		number[0] = '0';
		number[1] = '\0';
	}
	else{
		while(num){
			char x = num%10;
			number[i--] = x + '0';
			num /= 10;
		}
		number[dg] = '\0';
	}
}

// Printing 

void print_new_line(){
	if(next_line_index >= HEIGHT){
		// On revient au début de l'écran et on clear tout
		next_line_index = 0;
		clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
	}
	vga_index = WIDTH * next_line_index;
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


// Pixel manipulation

void change_pixel(int x, int y, char ch){
	vga_buffer[WIDTH*y+x] =  vga_entry(ch, g_fore_color, g_back_color);
}

void change_pixel_color(int x, int y, char ch, uint8 fore_color, uint8 back_color){
	vga_buffer[WIDTH*y+x] =  vga_entry(ch, fore_color, back_color);
}

void draw_rectangle(int tx, int ty, int bx, int by, char ch){ // 
	for(int y = by; y < ty; y++){
		for(int x = bx; x < tx; x++){
			vga_buffer[WIDTH*y+x] = vga_entry(ch, g_fore_color, g_back_color);
		}
	}
}

void draw_rectangle_color(int tx, int ty, int bx, int by, char ch, uint8 fore_color, uint8 back_color){ // 
	for(int y = by; y < ty; y++){
		for(int x = bx; x < tx; x++){
			vga_buffer[WIDTH*y+x] = vga_entry(ch, fore_color, back_color);
		}
	}
}

// test entry - TO BE DELETED LATER ON -
void test_entry()
{
	init_vga(WHITE,BLACK);
	print_new_line();
	print_string("Bienvenue sur le Vix-OS");
	print_new_line();
	print_string("Test");
	print_new_line();
	print_int(12345);
	clear_vga_buffer(&vga_buffer, WHITE,BLACK);
	print_char(0x3d);
	draw_rectangle(10,10,0,0,'@');
	draw_rectangle_color(20,20,10,10, '@', BLACK,WHITE);
}
