
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
 * 	prédecesseur (uint8*)
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
uint32 first_seg = 0x9100000;

//extern void* sbrk(uint32) ;

void* sbrk(uint32 sz){
	first_seg += sz;
	return (void*) first_seg;
}

// Initialise depuis un tableau de 30 pointeurs à la base du tas
void init_tas(void* base[30])
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
	return n ; 
}

// Taille du bloc en octets (hors entêtes)
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
void insert(void* base[30], uint8* next, uint8* new)
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
void maj_heads(void* base[30], uint8* bloc)
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
void link(void* base[30], uint8* bloc, uint32 n)
{
	if (n<=8) return ;
	uint32 k = log2(n) ;
	if (n<3<<k+2) insert(base, base[k], bloc) ;
	else insert(base, base[++k], bloc) ;
	maj_heads(base, bloc) ;
}

// Retire un bloc du système de ségrégation
void unlink(void* base[30], uint8* bloc)
{
	uint32 n = size(bloc) ;
	uint8** prev = *(uint8***) (bloc+n-4) ;
	uint8* next = *(uint8**) (bloc+n-8) ;
	uint32 n_next = (next==base[29]) ? 4 : size(next) ;
	*(uint8***) (next+n_next-4) = prev ;
	*prev = next ;
}

// Renvoie un bloc libre de taille >=n, NULL si inexistant
uint8* find(void* base[30], uint32 n)
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
void free(void* base[30], uint8* bloc)
{
	bloc -= 5 ;
	uint32 n = size(bloc) ;
	uint8* deb = bloc ;
	uint8* fin = bloc+n ;
	uint8* prev = deb-1 ;
	uint8* next = fin+1 ;
	if (*next & 1) {
		fin = next + size(next) ;
		unlink(base, next) ; }
	if (*prev & 1) {
		if (*prev & 8) deb -= *prev >> 4 ;
		else deb = **(uint8***) (prev-4) ;
		unlink(base, deb) ;	}
	n = init_bloc(deb, fin) ;
	link(base, deb, n) ;
}

// Alloue n octets sur le tas
void* malloc(void* base[30], uint32 n)
{
	uint8* bloc = find(base, n+5) ;
	if (bloc != NULL) {
		unlink(base, bloc) ;
		uint32 n_old = size(bloc) ;
		if (n_old>n+5) init_bloc(bloc+n+6, bloc+n_old) ; }
	else bloc = sbrk(n+6) ;
	*(bloc+n+5) = *bloc = 0	;
	*(uint32*) ++bloc = n ;
	return bloc+4 ;
}
