#include "../stdlib/stdlib.h"
#include "stddef.h" 
#include "structures.h"
#include "command_handler.h"
#include "../common/malloc.h"
#include "normal_automata.h"
#include "aho_corasick.h"
#include "syntax.h"

#define MINI_VIM_PROGRAM 

#define VIDEO_W 80
#define VIDEO_H 25-2 // On laisse 2 pour la baniere en bas

#define BUFF_SIZE 2000
#define LINE_NUMBER 100

#define REFRESH_RATE 100

file_t file;
line_t *screen_start;
text_list_t* current_text;
line_t* current_line;
int cursorX = 0;
int cursorY = 0; 

char* mode_text[4];
int mode_size[4];



void new_file()
{
	current_line = (line_t*) malloc(sizeof(line_t));
	file.filename = 0; // Nom de fichier vide
	file.file_buffer = current_line;
	current_line->size = 1;
	current_line->line_buffer = init_list((text_t) {' ', WHITE, BLACK, 0});
	current_line->prev = current_line->next = NULL; // Pas de suivant ni de précédent
	screen_start = current_line;
	current_text = current_line->line_buffer;
	current_text->e.cursor= 1;
}

void apply_move(direction_t dir)
{
	switch(dir)
	{
		case STILL:
			break;
		case UP:
			if(current_line->prev != NULL)
			{
				current_text->e.cursor = 0; // Il faut connaitre la position x et y
				current_line = current_line->prev;
				cursorX = cursorX < current_line->size ? cursorX : current_line->size-1;
				current_text = k_shift(current_line->line_buffer, cursorX);
				current_text->e.cursor = 1;
				cursorY--;
				if(cursorY < 0)
				{
					screen_start = current_line;
					cursorY = 0;
				}
			}
			break;
		case DOWN:
			if(current_line->next != NULL)
			{
				current_text->e.cursor = 0; // Il faut connaitre la position x et y
				current_line = current_line->next;
				cursorX = cursorX < current_line->size ? cursorX : current_line->size-1;
				current_text = k_shift(current_line->line_buffer, cursorX);
				current_text->e.cursor = 1;
				cursorY++;
				if(cursorY >= VIDEO_H)
				{
					screen_start = screen_start->next;
					cursorY = VIDEO_H;
				}

			}
			break;
		case RIGHT:
			if(current_text->next != NULL)
			{
				current_text->e.cursor = 0;
				current_text = current_text->next;
				current_text->e.cursor = 1;
				cursorX ++;
			}
			break;
		case LEFT:
			if(current_text->prev != NULL)
			{
				current_text->e.cursor = 0;
				current_text = current_text->prev;
				current_text->e.cursor = 1;
				cursorX--;
			}
			break;
	}
}

void apply_insert(direction_t dir)
{
	switch(dir)
	{
		case DOWN:
			insert_after_line(current_line, init_list((text_t) {' ', WHITE, BLACK, 0}));
			break;
		case RIGHT:
			if(current_text->next == NULL)
				insert_after(current_text, (text_t) {' ', WHITE, BLACK, 0});
	}
}

void apply_move_delete(direction_t dir)
{
	switch(dir)
	{
		case RIGHT:
			apply_move(LEFT);
			break;
		case DOWN:
			apply_move(UP);
			break;
	}
}

void apply_delete(int del_mode)
{
	if(del_mode == 1) // on va juste delete le caractere actuel
	{
		// Comment delete juste le caractere actuel
	if(current_text->next != NULL) 
	{
		int r = current_text->prev == NULL;
		current_text = current_text->next;
		current_text->e.cursor= 1;
		delete_node(current_text->prev);
		if(r)
		{
			current_line->line_buffer = current_text;
		}
	}
	else if(current_text->prev != NULL)
	{
		current_text = current_text->prev;
		current_text->e.cursor= 1;
		delete_node(current_text->next);
	}
	else// TODO : Faire un truc plus propre
	{ 
		current_text->e.c = ' ';
	}
	
	}
	if(del_mode == 3) // on va supprimer toute la ligne
	{
		if(current_line->next != NULL)
		{
			current_line = current_line->next; 
			if(current_line->prev->prev == NULL) // Si on était sur la premiere ligne
			{
				screen_start = current_line;
			}
			delete_node_line(current_line->prev);
		}
		else if(current_line->prev != NULL)
		{
			current_line = current_line->prev;
			delete_node_line(current_line->next);
			cursorY--;
		}
		else
		{
			current_line = (line_t*)malloc(sizeof(line_t));
			current_line->size = 1;
			current_line->line_buffer = init_list((text_t) {' ', WHITE, BLACK, 0});
			current_line->prev = current_line->next = NULL;
			file.file_buffer = current_line;
			screen_start = current_line;
		}
		current_text = current_line->line_buffer;
		current_text->e.cursor = 1;
	}
}
void* memset(void *pointer, int value, size_t count){
	uint8 *p = pointer;
	while(count--)
		*p++ = (uint8) value;
	return pointer;
}
void init_banner()
{
	mode_text[0] = "NORMAL";
	mode_text[1] = "INSERT";
	mode_text[2] = "REPLACE";
	mode_text[3] = "COMMAND";
	mode_size[0] = 6;
	mode_size[1] = 7;
	mode_size[2] = 8;
	mode_size[3] = 8;
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
	init_tas();
	int running = 1;
	init_banner();
	int line_under = 0; // la premiere ligne affichée à l'écran
	test_aho_corasick();

	automata_t automata = new_automata();

	// On va ajouter les commandes 
	// ---------------------
	// Commandes de déplacement
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=0, .mov=1, .dir=DOWN}, "j");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=0, .mov=1, .dir=UP}, "k");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=0, .mov=1, .dir=RIGHT}, "l");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=0, .mov=1, .dir=LEFT}, "h");
	// Commandes de suppression
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=3, .mov=1, .dir=DOWN}, "dj");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=3, .mov=1, .dir=UP}, "dk");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=1, .mov=1, .dir=RIGHT}, "dl");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=1, .mov=1, .dir=LEFT}, "dh");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=3, .mov=0, .dir=STILL}, "dd");
	add_command(&automata, (command_t) {.new_mode=NORMAL, .del=1, .mov=0, .dir=STILL}, "x");
	//Ajout en mode insertion
	add_command(&automata, (command_t) {.new_mode=INSERT, .del=0, .mov=1, .dir=DOWN}, "o");
	add_command(&automata, (command_t) {.new_mode=INSERT, .del=0, .mov=1, .dir=UP}, "O");
	add_command(&automata, (command_t) {.new_mode=INSERT, .del=0, .mov=1, .dir=RIGHT}, "a");
	// Commandes de changement de mode
	add_command(&automata, (command_t) {.new_mode=COMMAND,.del=0, .mov=0, .dir=STILL}, ":");
	add_command(&automata, (command_t) {.new_mode=INSERT, .del=0, .mov=0, .dir=STILL}, "i");
	// ---------------------


	// -- creatino du handler de syntaxe
	syntax_param_t param;
	param.fg = RED;
	param.bg = BLACK;
	char *lexemes[6] = {
											"\nint ", 
											"\nvoid ", 
											"\nchar ",
											"\nshort ",
											" int ", 
											" void ", 
											" char ",
											" short ",
											};
	trie_node_t* root = build_trie(6, lexemes);
	param.trie = (trie_iterator_t) {root, root};

	syntax_param_t param_stmt;
	param_stmt.fg = BLUE;
	param_stmt.bg = BLACK;
	char *stmts[12] = {
										"\nif ", 
										"\nelse ", 
										"\nsizeof ", 
										"\ncase ", 
										"\nwhile ", 
										"\nfor ",
										" if ", 
										" else ", 
										" sizeof ", 
										" case ", 
										" while ", 
										" for "
	};
	trie_node_t* root_stmt = build_trie(12, stmts);
	param_stmt.trie = (trie_iterator_t) {root_stmt, root_stmt};

	add_char(&param_stmt.trie, '\n');
	add_char(&param.trie, '\n');

	syntax_param_t parametres[2] = {param, param_stmt};

	syntax_config_t syntax = (syntax_config_t) {.n = 2, .conf = parametres};

	//


	sub_mode_t submode = NONE;
	//memset(buffer, 0, LINE_NUMBER * sizeof(line_t*));
	// Au départ il y a juste le premier qui est initialisé
	// Le reste est vide
	current_line = (line_t*) malloc(sizeof(line_t));
	file.filename = 0; // Nom de fichier vide
	file.file_buffer = current_line;
	current_line->size = 1;
	current_line->line_buffer = init_list((text_t) {' ', WHITE, BLACK, 0});
	current_line->prev = current_line->next = NULL; // Pas de suivant ni de précédent
	screen_start = current_line;
	mode_t current_mode = NORMAL;
	current_line->line_buffer->e.cursor = 1;
	current_text = current_line->line_buffer;
	
	char command_buffer[80]; // Le buffer pour contenir la commande en cours
	memset(command_buffer, 0, sizeof(char)*80);

	while(running){ // main loop
		// On va afficher à l'écran le buffer
		// Il faut peut etre flush l'écran à chaque rafraichissement
		// Ici on va faire la syntaxe

		syntax_from_start(&file, syntax);


		//
		//
		if(get_ticks() % REFRESH_RATE == 0){
			for(int i = 0; i < VIDEO_W; i++)
				for(int j = 0; j < VIDEO_H+2; j++)
					print_screen(i, j, ' ', WHITE, BLACK);
		}
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
		
		keyboard_t kp = get_keyboard();
		switch (current_mode){
			case NORMAL:
				if(kp.type == 0){ // On va faire un handler simple
				if(kp.k.ch)
				{
					command_t* command = enter_char(&automata, kp.k.ch);
					if(command) // On a bien recup une commande
					{
						//write(0, "\n");
						//print_int(command->mov);
						if(command->del) // mode de delete
						{
							// On va donc juste supprimmer la ligne
							apply_delete(command->del);
						}
						if(command->mov) // si on doit bouger
						{
							if(command->new_mode == INSERT)
							{
								apply_insert(command->dir);
							}
							apply_move(command->dir);
							if(command->del)
							{
								apply_move_delete(command->mov);
								apply_delete(command->del);
							}
						}
						current_mode = command->new_mode;
					}
				}
				break;
			}
			case INSERT:
				if(kp.type == 0 && kp.k.ch != 0){
					//On va ajouter au courant notre truc
					int premier = (current_text->prev == 0); // Si c'est le premier de la liste
					current_line->size++;
					text_list_t *nouveau = insert_before(current_text , (text_t) {kp.k.ch, WHITE, BLACK, 0});
					if(premier)
						current_line->line_buffer = nouveau;
				}
				else{
					if(kp.type == 1){
						int premier;
						text_list_t *nouveau;
						switch (kp.k.sp){
							case ESCAPE:
								current_mode = NORMAL;
								break;
							case SPACE:
								premier = (current_text->prev == 0); // Si c'est le premier de la liste
								current_line->size++;
								nouveau = insert_before(current_text , (text_t) {' ', WHITE, BLACK, 0});
								if(premier)
									current_line->line_buffer = nouveau;
								cursorX++;
								break;
							case TAB:
								premier = (current_text->prev == 0); // Si c'est le premier de la liste
								current_line->size++;
								nouveau = insert_before(current_text , (text_t) {' ', WHITE, BLACK, 0});
								if(premier)
									current_line->line_buffer = nouveau;
								cursorX++;
								premier = (current_text->prev == 0); // Si c'est le premier de la liste
								current_line->size++;
								nouveau = insert_before(current_text , (text_t) {' ', WHITE, BLACK, 0});
								if(premier)
									current_line->line_buffer = nouveau;
								cursorX++;
								break;
							case BACKSPACE: // On va supprimer le caractere d'avant
								if(current_text->prev != 0)
								{
									apply_move(LEFT);
									apply_delete(1);
								}
								else
								{
									int t = (current_line->prev==0);
									apply_delete(3);
									if(t)
										apply_move(UP);
								}
							break;
							case ENTER:
								//On va rajouter une ligne après celle ou on est
								current_text->e.cursor = 0;
								apply_insert(DOWN);
								apply_move(DOWN);
								current_text->e.cursor = 1;
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
					uint32 bm;
					switch (kp.k.sp){
						case ESCAPE:
							memset(command_buffer, 0, sizeof(char)*80);
							current_mode = NORMAL;
							break;
						case SPACE:
							command_buffer[i] = ' ';
							break;
						case BACKSPACE:
							if(i != 0)
								command_buffer[i-1]= 0;
							else
								current_mode = NORMAL;
							break;
						case ENTER:
							bm = interpret_command(command_buffer, &file);

							// La il faut interpreter la commande
							memset(command_buffer, 0, sizeof(char)*80);

							if(bm & 0x1) // On a eu une lecture de fichier
							{
								current_line = file.file_buffer;
								screen_start = current_line;
								current_text = current_line->line_buffer;
								current_text->e.cursor = 1;
								cursorX = 0;
								cursorY = 0;
							}
							if(bm & 0x2)  // Une demande d'arret
							{
								running = 0;
								write(0, "PROGRAM STOPPING");
							}
							if(bm & 0x4) // Une sauvegarde
							{

							}
							if(bm & 8) // C'est un nouveau fichier
							{
								new_file();
								//write(0, "OUVERTURE D'UN NOUVEAU FICHIER");
								cursorX = 0;
								cursorY = 0;
							}
							current_mode = NORMAL;
						break;
					}
				}

			break;


			}


	}
	
}
