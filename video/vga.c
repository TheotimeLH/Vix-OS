#include "vga.h"

// Index pour le buffer video
enum vga_mode current_mode = TEXT;
uint32 vga_index;
// compteur pour la prochaine ligne
static uint32 next_line_index = 1;
// Couleurs du fond et du premier plan
uint8 g_fore_color = WHITE, g_back_color = BLACK;
// Les chiffres en ascii
uint32 digit_ascii[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

void change_mode(enum vga_mode mode){
	switch(mode){
		case TEXT :
			asm("mov $0x03, %ax");
			asm("int $0x10");
			break;
		case GRAPHIC:
			asm("mov $0x12, %ax");
			asm("int $0x10");
			break;
		default:
			asm("mov $0x03, %ax");
			asm("int $0x10");
			break;
	}
}

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
void clear_vga_buffer_text(uint16 **buffer, uint8 fore_color, uint8 back_color){
	uint32 i;
	for(i = 0; i < BUFSIZE; i++)
		(*buffer)[i] = vga_entry(NULL,fore_color, back_color);
}

//initialize vga buffer 

void init_vga(uint8 fore_color, uint8 back_color){
//	if(current_mode != TEXT)
//		change_mode(TEXT);
	vga_buffer_text = (uint16*)VGA_ADDRESS_TEXT;
	clear_vga_buffer_text(&vga_buffer_text, fore_color, back_color);
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
	if(next_line_index >= TEXT_HEIGHT){
		// On revient au début de l'écran et on clear tout
		next_line_index = 0;
		clear_vga_buffer_text(&vga_buffer_text, g_fore_color, g_back_color);
	}
	vga_index = TEXT_WIDTH * next_line_index;
	next_line_index++;
}

void print_char(char ch){
	if(ch == '\n')
		print_new_line();
	else
		vga_buffer_text[vga_index++] = vga_entry(ch, g_fore_color, g_back_color);
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
	vga_buffer_text[TEXT_WIDTH*y + x] = vga_entry(c, g_fore_color, g_back_color);

}


// Pixel manipulation


void init_vga_graphic(uint8 fore_color, uint8 back_color){
	if(current_mode != GRAPHIC)
		change_mode(GRAPHIC);
	vga_buffer_graphic = (uint16*) VGA_ADDRESS_GRAPHIC;
	g_fore_color = fore_color;
	g_back_color = back_color;

}

void draw_pixel(int x, int y, uint8 color){
	vga_buffer_graphic[GRAPHIC_WIDTH*y+x] = color;
}

void draw_rectangle(int bx, int by, int tx, int ty, uint8 color){ // 
	for(int y = by; y < ty; y++){
		for(int x = bx; x < tx; x++){
			vga_buffer_graphic[GRAPHIC_WIDTH*y+x] = color;
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
	print_char(0x3d);
	print_new_line();
	print_string("Vim est le meilleur logiciel de tout les temps\n");
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
