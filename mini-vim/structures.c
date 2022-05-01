#include "structures.h"

// Pour les listes de lignes
line_t* init_list_line(text_list_t *element)
{
	line_t* liste = (line_t*) malloc(sizeof(line_t));
	liste->prev = liste->next = 0;
	liste->line_buffer = element;
	return liste;
}

line_t *insert_after_line(line_t* el, text_list_t *element)
{
	line_t* after = init_list_line(element);
	if(el->next != 0)
		el->next->prev = after;
	after->next = el->next;
	el->next = after;
	after->prev = el;
	return after;
}

line_t *insert_before_line(line_t* el, text_list_t *element) 
{
	line_t* before = init_list_line(element);
	if(el->prev != 0)
		el->prev->next = before;
	before->prev = el->prev;
	el->prev = before;
	before->next = el;
	return before;
}

line_t *k_shift_line(line_t* el, int k) // un déplacement de k cases
{
	line_t* p = el;
	if(k > 0){
		while(k-- && p->next) p = p->next;
		return p->next;
	}
	else{
		while(k++ && p->prev) p = p->prev;
		return p->prev;
	}
}	

void delete_node_line(line_t* el){
	if(el->prev != 0)
		el->prev->next = el->next;
	if(el->next != 0)
		el->next->prev = el->prev;
	// Il faudrait free la memoire
	//free((uint8*) el);
}


// Pour les listes de texte
text_list_t* init_list(text_t element)
{
	text_list_t* liste = (text_list_t*) malloc(sizeof(text_list_t));
	liste->prev = liste->next = 0;
	liste->e = element;
	return liste;
}

text_list_t *insert_after(text_list_t* el, text_t element)
{
	text_list_t* after = init_list(element);
	if(el->next != 0)
		el->next->prev = after;
	after->next = el->next;
	el->next = after;
	after->prev = el;
	return after;
}

text_list_t *insert_before(text_list_t* el, text_t element) 
{
	text_list_t* before = init_list(element);
	if(el->prev != 0)
		el->prev->next = before;
	before->prev = el->prev;
	el->prev = before;
	before->next = el;
	return before;
}

text_list_t *k_shift(text_list_t* el, int k) // un déplacement de k cases
{
	text_list_t* p = el;
	if(k > 0){
		while(k-- && p->next) p = p->next;
		return p->next;
	}
	else{
		while(k++ && p->prev) p = p->prev;
		return p->prev;
	}
}	

void delete_node(text_list_t* el){
	if(el->prev != 0)
		el->prev->next = el->next;
	if(el->next != 0)
		el->next->prev = el->prev;
	// Il faudrait free la memoire
	//free((uint8*) el);
}




