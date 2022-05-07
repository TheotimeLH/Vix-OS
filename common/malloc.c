
/* Allocation dynamique de mémoire:
 * 	Ségrégation des blocs libres
 * ________________________________
 *
 * Bloc alloué :
 * 	entête (0*8)
 * 	taille (uint32)
 * 	espace mémoire
 * 	entête (0*8)
 *
 * Bloc libre :
 * 	entête
 * 	taille (si >= 16)
 * 	- quelconque -
 * 	successeur (uint8*)
 * 	prédecesseur (uint8**)
 * 	entête
 *
 * Entête (8 bits) :
 * 	is_free
 * 	0
 * 	0
 * 	is_small (<16)
 * 	size:4 (if is_small)
 */

#include "common.h"
extern write(int, char*);
extern uint32 end ;
uint8* brk = &end ;
void* base[30] ;

// Inrémente la taille du tas
uint8* sbrk(uint32 n)
{
	return brk += n ;
}

// Initialise le tas
void init_tas()
{
	for (int i=0 ; i<29 ; i++) base[i] = base+29 ;
	base[29] = base ;
}

// Rempli l'entête d'un bloc libre
uint32 init_bloc(uint8* deb, uint8* fin)
{
	uint32 n = fin-deb ;
 	*fin = *deb = ((n&15)<<4) + ((n<16)<<3) +1 ;
	if (n >= 16) *(uint32*) ++deb = n ;
	link(deb, n) ;
}

// Taille du bloc en octets
uint32 size(uint8* bloc)
{
	if (*bloc & 8) return *bloc >> 4 ;
	else return *(uint32*) ++bloc ;
}	

// Indice de la liste adéquate
uint32 log2(uint32 n)
{
	uint32 k ;
	for (k=0 ; n<1<<k ; k++) ;
	return k-4 ;
}

// Insère un bloc avant un autre
void insert(uint8* next, uint8* new)
{
	uint32 n_new = size(new) ;
	uint32 n_next = (next==base[29]) ? 4 : size(next) ;
	uint8** prev = *(uint8***) (next+n_next-4) ;
	*(uint8***) (next+n_next-4) = (uint8**) new+n_new-8 ;
	*(uint8**) (new+n_new-8) = next ;
	*(uint8***) (new+n_new-4) = prev ;
	*(prev) = new ;
}

// Update les pointeurs têtes de listes
void maj_heads(uint8* bloc)
{
	uint32 n = size(bloc) ;
	uint8** prev = *(uint8***) (bloc+n-4) ;
	uint32 l = -1 ;
	if (prev != (void*) base) {
		prev = *(uint8***) ++prev ;
		l = log2(size(*prev)) ; }
	for (int i=l+1 ; i<=log2(n) ; i++) base[i]=bloc ;
}

// Ajoute un bloc dans le système de ségrégation
void link(uint8* bloc, uint32 n)
{
	if (n<=8) return ;
	uint32 k = log2(n) ;
	if (n<3<<k+2) insert(base[k], bloc) ;
	else insert(base[++k], bloc) ;
	maj_heads(bloc) ;
}

// Retire un bloc du système de ségrégation
void unlink(uint8* bloc)
{
	uint32 n = size(bloc) ;
	uint8** prev = *(uint8***) (bloc+n-4) ;
	uint8* next = *(uint8**) (bloc+n-8) ;
	uint32 n_next = (next==base[29]) ? 4 : size(next) ;
	*(uint8***) (next+n_next-4) = prev ;
	*prev = next ;
}

// Renvoie un bloc libre de taille >=n, NULL si inexistant
uint8* find(uint32 n)
{
	uint8* bloc = base[log2(n)] ;
	while (bloc != base[29]) {
		uint32 n_bloc = size(bloc) ;
		if (n_bloc >= n) return bloc ;
		bloc = *(uint8**) (bloc+n_bloc-8) ;
	}
	return NULL ;
}

// Libère un bloc alloué
void free(uint8* bloc)
{
	bloc -= 5 ;
	uint32 n = size(bloc) ;
	uint8* deb = bloc ;
	uint8* fin = bloc+n ;
	uint8* prev = deb-1 ;
	uint8* next = fin+1 ;
	if (*next & 1) {
		fin = next + size(next) ;
		unlink(next) ; }
	if (*prev & 1) {
		if (*prev & 8) deb -= *prev >> 4 ;
		else deb = **(uint8***) (prev-4) ;
		unlink(deb) ;	}
	init_bloc(deb, fin) ;
}

// Alloue n octets sur le tas
void* malloc(uint32 n)
{
	uint8* bloc = find(n+5) ;
	if (bloc != NULL) {
		unlink(bloc) ;
		uint32 n_old = size(bloc) ;
		if (n_old>n+5) init_bloc(bloc+n+6, bloc+n_old) ; }
	else {
		uint32 nbrk = n+6 ;
		if (*brk & 1) {
			if (*brk & 8) nbrk -= *brk >> 4 ;
			else brk -= size(**(uint8***) (brk-4)) ; }
		bloc = sbrk(nbrk)-n-5 ;	}
	*(bloc+n+5) = *bloc = 0	;
	*(uint32*) ++bloc = n ;
	return bloc+4 ;
}

