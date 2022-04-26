
/* Allocation dynamique de mémoire:
 * 	Ségrégation des blocs libres
 * ________________________________
 *
 * Bloc alloué :
 * 	entête
 * 	taille (uint32)
 * 	espace mémoire
 * 	entête
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
 * 	is_head
 * 	is_tail
 * 	is_small (<16)
 * 	size:4 (if is_small)
 */

#include "common.h"

extern void* sbrk(uint32) ;

// Initialise depuis un tableau de 30 pointeurs à la base du tas
void init_tas(uint8* base[30])
{
	*(base+29) = base ;
	for (int i=0 ; i<29 ; i++) base[i] = base+29 ;
}

// Rempli l'entête d'un bloc libre
uint32 init_bloc(uint8* deb, uint8* fin)
{
	uint32 n = fin-deb ;
 	*fin = *deb = ((n&15)<<4) + ((n<16)<<3) +1 ;
	if (n >= 16) *(uint32*) ++deb = n ;
	return n ; 
}

// Alloue n octets sur le tas
void* malloc(uint8** base, uint32 n)
{
	uint8* bloc = find_bloc(base[log2(n+5)], n+5) ;
	if (bloc == NULL) bloc = sbrk(n+6) ;
	else unlink(base, bloc) ;
	*(bloc+n+5) = *bloc = 0	;
	*(uint32*) ++bloc = n ;
	return bloc+4 ;
}

// Libère un bloc alloué
void free(uint8** base, uint8* bloc)
{
	bloc -= 5 ;
	uint32 n = size(bloc) ;
	uint8* deb(bloc), fin(bloc+n) ;
	uint8* prev(bloc-1), next(bloc+n+1) ;
	if (*next & 1) {
		fin = next + size(next) ;
		unlink(base, next) ; }
	if (*prev & 1) {
		if (*prev & 8) deb -= *prev >> 4 ;
		else deb = **(uint8**) (prev-4) ;
		unlink(base, deb) ;	}
	n = init_bloc(deb, fin) ;
	link(base, deb, n) ;
}

// Taille du bloc en octets (hors entêtes)
uint32 size(uint8** bloc)
{
	uint8 flags = *bloc ;
	if (flags & 8) return flags >> 4 ;
	else return *(uint32*) ++bloc ;
}	

uint32 log2(uint32 n)
{
	for (uint32 k=0 ; n<1<<k ; k++) ;
	return k-4 ;
}

// Insère un bloc dans le système de ségrégation
void link(uint8** base, uint8* bloc, uint32 n)
{
	if (n<=8) return ;
	uint32 k = log2(n) ;
	if (n<3<<k-2) insert(base, base[k-1], bloc) ;
	else insert(base, base[k], bloc) ;
	maj_heads(base, bloc) ;
}

// Insère un bloc avant un autre
void insert(uint8** base, uint8* next, uint8* new)
{
	uint32 n_new = size(new) ;
	uint32 n_next = next==base[29] ? 4 : size(next) ;
	uint8* prev = *(next+n_next-4) ;
	*(next+n_next-4) = new+n_new-8 ;
	*(new+n_new-8) = next ;
	*(new+n_new-4) = prev ;
	*(prev) = new ;
}

// Update les pointeurs têtes de listes
void maj_heads(uint8** base, uint8* bloc)
{
	uint32 n = size(bloc) ;
	uint8* prev = *(bloc+n-4) ;
	uint32 l = -1 ;
	if (prev != *base) {
		prev = **(uint8**) (prev+4) ;
		l = log2(size(prev)) ; }
	for (int i=l+1 ; i<=log2(n) ; i++) base[i]=bloc ;
}

// Retire un bloc du système de ségrégation
void unlink(uint8** base, uint8* bloc)
{
	uint32 n = size(bloc) ;
	uint8* prev = *(bloc+n-4) ;
	uint8* next = *(bloc+n-8) ;
	uint32 n_next = next==base[29] ? 4 : size(next) ;
	*(next+n_next-4) = prev ;
	*prev = next ;
}

// Renvoie un bloc libre de taille >=n, NULL si inexistant
uint8* find(uint8* bloc, uint32 n)
{
	if (bloc==base[29]) return NULL ;
	uint32 nb = size(bloc) ;
	if (nb<n) return find(*(bloc+n-8), n) ;
	else return bloc ;
}

