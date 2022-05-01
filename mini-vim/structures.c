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
	el->next->prev = after;
	after->next = el->next;
	el->next = after;
	after->prev = el;
	return after;
}

list_t *insert_before(list_t* el, text_t element) 
{
	list_t* before = init_list(element);
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

