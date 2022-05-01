#include "../stdlib/stdlib.h"
#include "../common/malloc.h"

typedef struct text{
	char c;
	uint8 fg; // La couleur du premier plan
	uint8 bg; // La couleur de l'arriere plan
	uint8 cursor; // Le curseur est-il dessus
} text_t;

typedef struct list{
	struct list *prev, *next;
	text_t e;
}list_t;

typedef struct line{
	uint32 size;
	list_t* line_buffer;
}line_t;



typedef enum mode{
	NORMAL,
	INSERT,
	REPLACE,
} mode_t;

list_t *init_list(text_t element);
list_t *insert_after(list_t *el, text_t element);
list_t *insert_before(list_t *el, text_t element);
list_t *k_shift(list_t* el, int k);
void delete_node(list_t* el);
