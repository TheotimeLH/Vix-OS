#include "../stdlib/stdlib.h"
#include "stddef.h" 
#include "structures.h"
#include "../common/malloc.h"


#define VIDEO_W 80
#define VIDEO_H 25-2 // On laisse 2 pour la baniere en bas

#define BUFF_SIZE 2000
#define LINE_NUMBER 100

char* mode_text[3];


void* memset(void *pointer, int value, size_t count){
	uint8 *p = pointer;
	while(count--)
		*p++ = (uint8) value;
	return pointer;
}
void init_banner()
{
	mode_text[0] = (char*) malloc(sizeof(char)*6);
	mode_text[1] = (char*) malloc(sizeof(char)*7);
	mode_text[2] = (char*) malloc(sizeof(char)*8);
	char t1[6] = "NORMAL";
	char t2[7] = "INSERT";
	char t3[8] = "REPLACE";
	for(int i = 0; i < 6; i++)
		mode_text[0][i] = t1[i];
	for(int i = 0; i < 7; i++)
		mode_text[1][i] = t2[i];
	for(int i = 0; i < 8; i++)
		mode_text[2][i] = t3[i];
}
void render_banner(mode_t current_mode)
{
	
	for(int i = current_mode+6; i < VIDEO_W; i++)
	{
		print_screen(i, VIDEO_H, ' ', BLACK, WHITE);
	}
	for(int i = 0; i < current_mode+6; i++)
		print_screen(i, VIDEO_H, mode_text[current_mode][i], BLACK,WHITE);
}
int main(){
	// Donc déjà il faut lire un fichier (mais bon ça c'est pour plus tard
	//
	init_tas();
	init_banner();
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
		if(get_ticks() % 100 == 0){
			for(int i = 0; i < VIDEO_W; i++)
				for(int j = 0; j < VIDEO_H+2; j++)
					print_screen(i, j, ' ', WHITE, BLACK);
		}
		/*
		print_screen(3, 3, current_buff->e.c, current_buff->e.fg, current_buff->e.bg);
		if(current_buff->prev != 0)
				print_screen(4, 3, current_buff->prev->e.c, current_buff->prev->e.fg, current_buff->prev->e.bg);
		*/
		render_banner(current_mode);
		//
		//
		int nb_line_aff = 0; // On ne doit pas depasser VIDEO_H
		int curr_line = 0;
		line_t* ac_line = buffer[line_under];
		uint32 posX = 0, posY = 0; // on commence à afficher en haut à gauche de l'écran
		while(nb_line_aff < VIDEO_H && ac_line != 0){
			// ON va afficher la ligne actuelle  (en sautant à la ligne si jamais, on atteint le bout)
			list_t* it = ac_line->line_buffer; // normalement, il y a autant d'element que size
			while(it != 0){
				text_t ac = it->e;
				if(ac.cursor){
					print_screen(posX, posY, ac.c, ac.bg, ac.fg);
				}
				else{
					print_screen(posX, posY, ac.c, ac.fg, ac.bg);
				}
				posX++;
				it = it->next;
				if(posX >= VIDEO_W){
					posY++;
					nb_line_aff++;
					posX =0;
				}
			}
			posY++;
			posX = 0;
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
							if(current_buff->prev != 0)
							{
								current_buff->e.cursor = 0;
								current_buff = current_buff->prev;
								current_buff->e.cursor = 1;
							}
							break;
						case 'k': // On va en haut
							// TODO
							break;
						case 'j': // On va en bas
							// TODO
							break;
						case 'l': // On va à droite
							if(current_buff->next != 0)
							{
								current_buff->e.cursor = 0;
								current_buff = current_buff->next;
								current_buff->e.cursor = 1;
							}
							break;
						case 'i': // On passe en insert mode
							current_mode = INSERT;
							break;
						case 'x': //
							if(current_buff->next != 0) 
							{
								current_buff = current_buff->next;
								current_buff->e.cursor= 1;
								delete_node(current_buff->prev);
							}
							else if(current_buff->prev !=0)
							{
								current_buff = current_buff->prev;
								current_buff->e.cursor= 1;
								delete_node(current_buff->next);
							}
							else// TODO : Faire un truc plus propre
							{ 
								current_buff->e.c = ' ';
							}
						break;
					}
				break;
			}
			case INSERT:
				if(kp.type == 0 && kp.k.ch != 0){
					//On va ajouter au courant notre truc
					int premier = (current_buff->prev == 0); // Si c'est le premier de la liste
					current->size++;
					list_t *nouveau = insert_before(current_buff , (text_t) {kp.k.ch, WHITE, BLACK, 0});
					if(premier)
						current->line_buffer = nouveau;
				}
				else{
					if(kp.type == 1){
						switch (kp.k.sp){
							case ESCAPE:
								current_mode = NORMAL;
								break;
							case SPACE:
								int premier = (current_buff->prev == 0); // Si c'est le premier de la liste
								current->size++;
								list_t *nouveau = insert_before(current_buff , (text_t) {' ', WHITE, BLACK, 0});
								if(premier)
									current->line_buffer = nouveau;
								break;
							case BACKSPACE: // On va supprimer le caractere d'avant
								if(current_buff->prev != 0)
								{
									if(current_buff->prev->prev == 0)
									{
										current->line_buffer = current_buff;
									}
									delete_node(current_buff->prev);
								}
								else
									current_buff->e.c = ' ';
						}
					}
				}

			break;


			}


	}
	
}
