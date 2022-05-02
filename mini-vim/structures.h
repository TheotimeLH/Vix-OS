#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "../stdlib/stdlib.h"
#include "../common/malloc.h"

typedef struct pair
{
	void* fst;
	void* snd;
} pair_t;



typedef struct text{
	char c;
	uint8 fg; // La couleur du premier plan
	uint8 bg; // La couleur de l'arriere plan
	uint8 cursor; // Le curseur est-il dessus
} text_t;

typedef struct list{
	struct list *prev, *next;
	text_t e;
}text_list_t;

typedef struct line{
	uint32 size;
	struct line *prev, *next;
	text_list_t* line_buffer;
}line_t;


typedef enum mode{
	NORMAL = 0,
	INSERT,
	REPLACE,
	COMMAND,
} mode_t;

line_t *init_list_line(text_list_t *element);
line_t *insert_after_line(line_t *el, text_list_t *element);
line_t *insert_before_line(line_t *el, text_list_t *element);
line_t *k_shift_line(line_t* el, int k);
void delete_node_line(line_t* el);

text_list_t *init_list(text_t element);
text_list_t *insert_after(text_list_t *el, text_t element);
text_list_t *insert_before(text_list_t *el, text_t element);
text_list_t *k_shift(text_list_t* el, int k);
void delete_node(text_list_t* el);


#endif
