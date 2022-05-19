#include "buffer_handler.h"


line_t* buffer_from_file(char* filename, uint32 size){
	char *buff = malloc(size*sizeof(char)); 
	//write(0, filename);
	uint32 file = open(filename);
	if(file != (uint32) -1)
	{
		uint32 size_file = read(file, buff, size);
		buff[size_file+1] = 0;
		// On va maintenant faire un truc sympa
		line_t* final = (line_t*) malloc(sizeof(line_t));
		// On va d'abord lire la premiere ligne
		final->size = 0;
		final->prev = final->next = 0;
		line_t* actuel = final;
		int i = -1;
		text_list_t* current_text_list = init_list((text_t) {' ', WHITE, BLACK, 0});
		text_list_t* current_pos = current_text_list;
		while(buff[++i])
		{
			if(buff[i] == '\n')
			{
				// On saute une ligne
				line_t* next_line = malloc(sizeof(line_t));
				next_line->prev = actuel;
				next_line->next = 0;
				current_text_list = current_text_list->next;
				delete_node(current_text_list->prev);
				actuel->line_buffer = current_text_list;
				actuel->next = next_line;
				actuel = next_line;
				current_text_list = init_list((text_t) {' ', WHITE, BLACK, 0});
				current_pos = current_text_list;
			}
			else
			{
				actuel->size++;
				current_pos = insert_after(current_pos, (text_t) {buff[i], WHITE, BLACK, 0});
			}
		}
		return final;
	}
	else{
		line_t * final = (line_t*) malloc(sizeof(line_t));
		text_list_t* cur = init_list((text_t) {' ', WHITE, BLACK, 0});
		final->size = 0;
		final->prev = final->next = 0;
		final ->line_buffer = cur;
		return final;
	}
}

static char* to_string(line_t* file)
{
	// On détermine d'abord la taille
	line_t* actuel = file;
	uint32 size = 0;
	int continuer = 1;
	while(actuel != 0) // On pourrait faire confiance à la size donnée dans les lignes 
	{
		text_list_t* ligne = actuel->line_buffer;
		while(ligne)
		{
			ligne = ligne->next;
			size++;
		}
		actuel = actuel->next;
		size++; // Caractere de saut de ligne
	}
	//On va maintenant calculer le buffer
	
	char* buff = (char*) malloc(sizeof(char)*size);
	int i;
	actuel = file;
	while(actuel)
	{
		text_list_t* ligne = actuel->line_buffer;
		while(ligne)
		{
			buff[i++] = ligne->e.c;
			ligne = ligne->next;
		}
		actuel = actuel->next;
		buff[i++]  = '\n';
	}
	return buff;
}
void save_to_file(char* filename, line_t* file)
{
	uint32 f = open(filename);
	char* file_data = to_string(file);
	write(0, file_data);
	write(f, file_data);
}
