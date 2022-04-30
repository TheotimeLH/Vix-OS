#include "../stdlib/stdlib.h"
#include "stddef.h"

#define VIDEO_W 80
#define VIDEO_H 25

char* buffer = 0; // Au départ on a un buffer vide

typedef struct text{
	char c;
	uint8 fg; // La couleur du premier plan
	uint8 bg; // La couleur de l'arriere plan
	uint8 cursor; // Le curseur est-il dessus
} text_t;


typedef struct line{
	text_t* line_buffer;
}line_t;

void* memset(void *pointer, int value, size_t count){
	uint8 *p = pointer;
	while(count--)
		*p++ = (uint8) value;
	return pointer;
}

int main(){
	// Donc déjà il faut lire un fichier (mais bon ça c'est pour plus tard
	//
	int line_under = 0; // la premiere ligne affichée à l'écran
	text_t buffer[100000]; // Pour l'instant on a juste un buffer alloué n'importe comment, il faudra utiliser malloc
	memset(buffer, 0, sizeof(text_t)*10000);
	int cursorX = 0, cursorY = 0;
	for(int i = 0; i < 100000; i ++){
		buffer[i].c = ' ';
		buffer[i].fg = WHITE;
		buffer[i].bg = BLACK;
		buffer[i].cursor = 0;
	}
	buffer[0].cursor = 1;
	
	while(1){ // main loop
		// On va afficher à l'écran le buffer
		int s = line_under * VIDEO_W;
		for(int i = 0; i < (VIDEO_H -2)*VIDEO_W; i++){
			text_t ac = buffer[i+s];
			if(ac.cursor){
				print_screen(i%VIDEO_W, i/VIDEO_W, ac.c, ac.bg, ac.fg);
			}
			else{
				print_screen(i%VIDEO_W, i/VIDEO_W, ac.c, ac.fg, ac.bg);
			}
		}
		
		// On a pas encore les gestions claviers
		// Il faudrait plutot considerer ça ligne par ligne, parce que la on a le probleme que chaque ligne fait au plus 80 caracteres...
		keyboard_t kp = get_keyboard();

		if(kp.type == 0){ // On va faire un handler simple
			switch (kp.k.ch){
				case 'h': // On va à gauche
					if(cursorX > 0) 
					{
						buffer[(cursorX--)+ (cursorY + line_under)*VIDEO_W].cursor = 0;  ;
						buffer[(cursorX)+ (cursorY + line_under)*VIDEO_W].cursor = 1;  ;
					}
					break;
				case 'k': // On va en haut
					if(cursorY > 0) 
					{
						buffer[(cursorX)+ ((cursorY--) + line_under)*VIDEO_W].cursor = 0;  ;
						buffer[(cursorX)+ (cursorY + line_under)*VIDEO_W].cursor = 1;  ;
					}
					break;
				case 'j': // On va en bas
					buffer[(cursorX)+ ((cursorY++) + line_under)*VIDEO_W].cursor = 0;  ;
					buffer[(cursorX)+ (cursorY + line_under)*VIDEO_W].cursor = 1;  ;
					break;
				case 'l': // On va à droite
					if(cursorX < VIDEO_W -1) 
					{
						buffer[(cursorX++)+ (cursorY + line_under)*VIDEO_W].cursor = 0;  ;
						buffer[(cursorX)+ (cursorY + line_under)*VIDEO_W].cursor = 1;  ;
					}
					break;
			}

		}


	}
	
}