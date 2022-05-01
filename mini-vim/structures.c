#include "structures.h"



list_t* init_list(text_t element)
{
	list_t* liste = (list_t*) malloc(sizeof(list_t));
	liste->prev = liste->next = 0;
	liste->e = element;
	return liste;
}

list_t *insert_after(list_t* el, text_t element)
{
	list_t* after = init_list(element);
	if(el->next != 0)
		el->next->prev = after;
	after->next = el->next;
	el->next = after;
	after->prev = el;
	return after;
}

list_t *insert_before(list_t* el, text_t element) 
{
	list_t* before = init_list(element);
	if(el->prev != 0)
		el->prev->next = before;
	before->prev = el->prev;
	el->prev = before;
	before->next = el;
	return before;
}

list_t *k_shift(list_t* el, int k) // un déplacement de k cases
{
	list_t* p = el;
	if(k > 0){
		while(k-- && p->next) p = p->next;
		return p->next;
	}
	else{
		while(k++ && p->prev) p = p->prev;
		return p->prev;
	}
}	

void delete_node(list_t* el){
	if(el->prev != 0)
		el->prev->next = el->next;
	if(el->next != 0)
		el->next->prev = el->prev;
	// Il faudrait free la memoire
	//free((uint8*) el);
}

