#include "../stdlib/stdlib.h"
#include "stddef.h"
#include "structures.h"
#include "../common/malloc.h"


#define VIDEO_W 80
#define VIDEO_H 25

#define BUFF_SIZE 2000
#define LINE_NUMBER 100

void* memset(void *pointer, int value, size_t count){
	uint8 *p = pointer;
	while(count--)
		*p++ = (uint8) value;
	return pointer;
}

int main(){
	// Donc déjà il faut lire un fichier (mais bon ça c'est pour plus tard
	//
	init_tas();
	int line_under = 0; // la premiere ligne affichée à l'écran
	//text_tab_t buffer[BUFF_SIZE]; // Pour l'instant on a juste un buffer alloué n'importe comment, il faudra utiliser malloc
	line_t* buffer[LINE_NUMBER];
	memset(buffer, 0, LINE_NUMBER* sizeof(line_t*));
	int cursorX = 0, cursorY = 0;
	// Au départ il y a juste le premier qui est initialisé
	// Le reste est vide
	
	line_t *b0 = (line_t*) malloc(sizeof(line_t));
	b0->size = 1;
	b0->line_buffer = init_list(' ');

	mode_t current_mode = NORMAL;
	
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
		switch (current_mode){
			case NORMAL:
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
						case 'i': // On passe en insert mode
							current_mode = INSERT;
							break;
					}
				break;
			}
			case INSERT:
				if(kp.type == 0 && kp.k.ch != 0){
					buffer[(cursorX)+ (cursorY + line_under)*VIDEO_W].c = kp.k.ch; 
					buffer[(cursorX++)+ (cursorY + line_under)*VIDEO_W].cursor = 0;  
					buffer[(cursorX)+ (cursorY + line_under)*VIDEO_W].cursor = 1;  
					if(cursorX >= VIDEO_W){
						cursorX =0;
						cursorY++;
					}
					if(cursorY == VIDEO_H){
						line_under++;
						cursorY--;
					}
				}
				else{
					if(kp.k.sp == ESCAPE){
						current_mode = NORMAL;
					}
				}

			break;


			}


	}
	
}
