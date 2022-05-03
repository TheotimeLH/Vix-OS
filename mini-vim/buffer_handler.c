#include "buffer_handler.h"


line_t* buffer_from_file(char* filename, uint32 size){
	char buff[size]; 
	//write(0, filename);
	uint32 file = open(filename);
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
