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
	memset(buffer, 0, LINE_NUMBER * sizeof(line_t*));
	int cursorX = 0, cursorY = 0;
	// Au départ il y a juste le premier qui est initialisé
	// Le reste est vide
	
	line_t *current = (line_t*) malloc(sizeof(line_t));
	current->size = 1;
	current->line_buffer = init_list((text_t) {' ', WHITE, BLACK, 0});

	mode_t current_mode = NORMAL;
	current->line_buffer->e.cursor = 1;
	list_t *current_buff = current->line_buffer;
	buffer[0] = current;
	print_screen(5, 5, 'T',WHITE, BLACK); 
	while(1){ // main loop
		// On va afficher à l'écran le buffer
		// Il faut peut etre flush l'écran à chaque rafraichissement
		if(get_ticks() % 5 == 0){
			for(int i = 0; i < VIDEO_W; i++)
				for(int j = 0; j < VIDEO_H; j++)
					print_screen(i, j, ' ', WHITE, BLACK);
		}
		//
		int nb_line_aff = 0; // On ne doit pas depasser VIDEO_H
		int curr_line = 0;
		line_t* ac_line = buffer[line_under];
		uint32 posX = 0, posY = 0; // on commence à afficher en haut à gauche de l'écran
		while(nb_line_aff < VIDEO_H){
			// ON va afficher la ligne actuelle  (en sautant à la ligne si jamais, on atteint le bout)
			list_t* it = ac_line->line_buffer; // normalement, il y a autant d'element que size
			for(int i = 0; i < ac_line->size; i++){
				text_t ac = it->e;
				if(ac.cursor){
					print_screen(posX, posY, ac.c, ac.bg, ac.fg);
				}
				else{
					print_screen(posX, posY, ac.c, ac.fg, ac.bg);
				}
				posX++;
				if(posX >= VIDEO_W){
					posY++;
					nb_line_aff++;
					posX =0;
				}
			}
			posY++;
			nb_line_aff++;
			ac_line = buffer[++curr_line];
			if(ac_line == 0)
				break;
		}



		/*
		for(int i = 0; i < (VIDEO_H -2)*VIDEO_W; i++){
			if(ac.cursor){
				print_screen(i%VIDEO_W, i/VIDEO_W, ac.c, ac.bg, ac.fg);
			}
			else{
				print_screen(i%VIDEO_W, i/VIDEO_W, ac.c, ac.fg, ac.bg);
			}
		}
		*/
		
		// On a pas encore les gestions claviers
		// Il faudrait plutot considerer ça ligne par ligne, parce que la on a le probleme que chaque ligne fait au plus 80 caracteres...
		keyboard_t kp = get_keyboard();
		switch (current_mode){
			case NORMAL:
				if(kp.type == 0){ // On va faire un handler simple
					switch (kp.k.ch){
						case 'h': // On va à gauche
							// TODO
							break;
						case 'k': // On va en haut
							// TODO
							break;
						case 'j': // On va en bas
							// TODO
							break;
						case 'l': // On va à droite
							// TODO
							break;
						case 'i': // On passe en insert mode
							current_mode = INSERT;
							break;
					}
				break;
			}
			case INSERT:
				if(kp.type == 0 && kp.k.ch != 0){
					//On va ajouter au courant notre truc
					current_buff->e.cursor = 0;
					current_buff = insert_after(current , (text_t) {kp.k.ch, WHITE, BLACK, 1});
				}
				else{
					if(kp.type == 1){
						if(kp.k.sp == ESCAPE)
							current_mode = NORMAL;
					}
				}

			break;


			}


	}
	
}
