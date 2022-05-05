#include "../stdlib/stdlib.h"
#include "stddef.h" 
#include "structures.h"
#include "command_handler.h"
#include "../common/malloc.h"

#define VIDEO_W 80
#define VIDEO_H 25-2 // On laisse 2 pour la baniere en bas

#define BUFF_SIZE 2000
#define LINE_NUMBER 100

char* mode_text[4];
int mode_size[4];


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
	mode_text[3] = (char*) malloc(sizeof(char)*8);
	mode_size[0] = 6;
	mode_size[1] = 7;
	mode_size[2] = 8;
	mode_size[3] = 8;
	char t1[6] = "NORMAL";
	char t2[7] = "INSERT";
	char t3[8] = "REPLACE";
	char t4[8] = "COMMAND";
	for(int i = 0; i < 6; i++)
		mode_text[0][i] = t1[i];
	for(int i = 0; i < 7; i++)
		mode_text[1][i] = t2[i];
	for(int i = 0; i < 8; i++)
		mode_text[2][i] = t3[i];
	for(int i = 0; i < 8; i++)
		mode_text[3][i] = t4[i];
}
void render_banner(mode_t current_mode)
{
	
	for(int i = mode_size[current_mode]; i < VIDEO_W; i++)
	{
		print_screen(i, VIDEO_H, ' ', BLACK, WHITE);
	}
	for(int i = 0; i < mode_size[current_mode]; i++)
		print_screen(i, VIDEO_H, mode_text[current_mode][i], BLACK,WHITE);
}
int main(){
	// Donc déjà il faut lire un fichier (mais bon ça c'est pour plus tard
	//
	int running = 1;
	init_tas();
	init_banner();
	int line_under = 0; // la premiere ligne affichée à l'écran
	//text_tab_t buffer[BUFF_SIZE]; // Pour l'instant on a juste un buffer alloué n'importe comment, il faudra utiliser malloc
	//line_t* buffer[LINE_NUMBER];

	write(0, "Lancement de MVIM");
	sub_mode_t submode = NONE;
	//memset(buffer, 0, LINE_NUMBER * sizeof(line_t*));
	int cursorX = 0, cursorY = 0;
	// Au départ il y a juste le premier qui est initialisé
	// Le reste est vide
	file_t file;
	line_t *current = (line_t*) malloc(sizeof(line_t));
	file.filename = 0; // Nom de fichier vide
	file.file_buffer = current;
	current->size = 1;
	current->line_buffer = init_list((text_t) {' ', WHITE, BLACK, 0});
	current->prev = current->next = NULL; // Pas de suivant ni de précédent
	line_t *screen_start = current;
	mode_t current_mode = NORMAL;
	current->line_buffer->e.cursor = 1;
	text_list_t *current_buff = current->line_buffer;
	print_screen(5, 5, 'T',WHITE, BLACK); 
	line_t* file_begin = screen_start; 
	
	char command_buffer[80]; // Le buffer pour contenir la commande en cours
	memset(command_buffer, 0, sizeof(char)*80);

	while(running){ // main loop
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
		line_t* ac_line = screen_start;
		uint32 posX = 0, posY = 0; // on commence à afficher en haut à gauche de l'écran
		while(nb_line_aff < VIDEO_H && ac_line != 0){
			// ON va afficher la ligne actuelle  (en sautant à la ligne si jamais, on atteint le bout)
			text_list_t* it = ac_line->line_buffer; // normalement, il y a autant d'element que size
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
			ac_line = ac_line->next;
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
							if(current_buff->prev != NULL)
							{
								current_buff->e.cursor = 0;
								current_buff = current_buff->prev;
								current_buff->e.cursor = 1;
								cursorX--;
							}
							break;
						case 'k': // On va en haut
							if(current->prev != NULL)
							{
								current_buff->e.cursor = 0; // Il faut connaitre la position x et y
								current = current->prev;
								cursorX = cursorX < current->size ? cursorX : current->size-1;
								current_buff = k_shift(current->line_buffer, cursorX);
								current_buff->e.cursor = 1;
							}
							break;
						case 'j': // On va en bas
							if(current->next != NULL)
							{
								current_buff->e.cursor = 0; // Il faut connaitre la position x et y
								current = current->next;
								cursorX = cursorX < current->size ? cursorX : current->size-1;
								current_buff = k_shift(current->line_buffer, cursorX);
								current_buff->e.cursor = 1;
							}
							break;
						case 'l': // On va à droite
							if(current_buff->next != NULL)
							{
								current_buff->e.cursor = 0;
								current_buff = current_buff->next;
								current_buff->e.cursor = 1;
								cursorX ++;
							}
							break;
						case 'i': // On passe en insert mode
							current_mode = INSERT;
							break;
						case 'x': //
							if(current_buff->next != NULL) 
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
						case 'o': // On va ajouter une ligne après et passer en INSERT mode
							current_buff->e.cursor = 0;
							current = insert_after_line(current, init_list((text_t) {' ', WHITE, BLACK, 0}));
							current_buff = current->line_buffer;
							current_buff->e.cursor = 1;
							current_mode = INSERT;
							cursorX = 0;
						break;
						case 'd':
							if(submode == NORMAL)
								submode = DELETE;
							else if (submode == DELETE)
							{ // on va supprimer la ligne actuelle
								if(current->next)
								{
									current = current->next;
									delete_node_line(current->prev);
								}
								else if(current->prev)
								{
									current = current->prev;
									delete_node_line(current->next);
								}
								else
								{
									current = (line_t*)malloc(sizeof(line_t));
									current->size = 1;
									current->line_buffer = init_list((text_t) {' ', WHITE, BLACK, 0});
									current->prev = current->next = NULL;
									file.file_buffer = current;
									screen_start = current;
								}
								current_buff = current->line_buffer;
								current_buff->e.cursor = 1;
								submode = NORMAL;
							}
						break;
						case ':':
							// On passe en mode commande
							current_mode = COMMAND;
						
						break;
					}
				break;
			}
			case INSERT:
				if(kp.type == 0 && kp.k.ch != 0){
					//On va ajouter au courant notre truc
					int premier = (current_buff->prev == 0); // Si c'est le premier de la liste
					current->size++;
					text_list_t *nouveau = insert_before(current_buff , (text_t) {kp.k.ch, WHITE, BLACK, 0});
					if(premier)
						current->line_buffer = nouveau;
				}
				else{
					if(kp.type == 1){
						int premier;
						switch (kp.k.sp){
							case ESCAPE:
								current_mode = NORMAL;
								break;
							case SPACE:
								premier = (current_buff->prev == 0); // Si c'est le premier de la liste
								current->size++;
								text_list_t *nouveau = insert_before(current_buff , (text_t) {' ', WHITE, BLACK, 0});
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
									current->size--;
								}
								else
									current_buff->e.c = ' ';
							break;
							case ENTER:
								//On va rajouter une ligne après celle ou on est
								current_buff->e.cursor = 0;
								current = insert_after_line(current, init_list((text_t) {' ', WHITE, BLACK, 0}));
								current_buff = current->line_buffer;
								current_buff->e.cursor = 1;
								cursorX = 0;
							break;


						}
					}
				}

			break;
			case COMMAND :
				print_screen(0, 24, ':', WHITE, BLACK); // ON affiche le truc
				int i = -1;
				while(command_buffer[++i]) print_screen(1+i, 24, command_buffer[i], WHITE, BLACK);
				if(kp.type == 0) // ON rajoute un truc à la commande
				{
					command_buffer[i] = kp.k.ch;
				}
				else
				{
					switch (kp.k.sp){
						case ESCAPE:
							memset(command_buffer, 0, sizeof(char)*80);
							current_mode = NORMAL;
							break;
						case SPACE:
							command_buffer[i] = ' ';
							break;
						case BACKSPACE:
							command_buffer[i-1]= 0;
							break;
						case ENTER:
							// La il faut interpreter la commande
							uint32 bm = interpret_command(command_buffer, &file);
							memset(command_buffer, 0, sizeof(char)*80);

							if(bm & 0x1) // On a eu une lecture de fichier
							{
								current = file.file_buffer;
								screen_start = current;
								current_buff = current->line_buffer;
								current_buff->e.cursor = 1;
								file_begin = screen_start;
								cursorX = 0;
							}
							if(bm & 0x2)  // Une demande d'arret
							{
								running = 0;
								write(0, "PROGRAM STOPPING");
							}
							if(bm & 0x4) // Une sauvegarde
							{

							}
							current_mode = NORMAL;
						break;
					}
				}

			break;


			}


	}
	
}
