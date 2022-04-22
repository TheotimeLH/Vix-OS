
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
	*base = base+30 ;
	for (int i=1 ; i<30 ; i++) base[i] = NULL ;
}

// Rempli l'entête d'un bloc libre
uint32 init_bloc(uint8* deb, uint8* fin)
{
	uint32 n = fin-deb-1 ;
	if (n >= 16) *(uint32*) ++deb = n ;
 	*fin = *deb = ((n&15)<<4) + ((n<16)<<3) +1 ;
	return n ; 
}

// Alloue n octets sur le tas
void* malloc(uint8** base, uint32 n)
{
	uint8* bloc = find_bloc(base, n+4) ;
	if (bloc != NULL) unlink(base, bloc) ;
	else bloc = sbrk(n+6) ;
	*(bloc+n+5) = *bloc = 0	;
	*(uint32*) ++bloc = n ;
	return bloc+4 ;
}

// Libère un bloc alloué
void free(uint8** base, uint8* bloc)
{
	bloc -= 5 ;
	uint32 n = size(bloc) ;
	uint8* deb(bloc), fin(bloc+n+1) ;
	uint8* prev(bloc-1), next(bloc+n+2) ;
	if (*next & 1) {
		fin = next + size(next) + 1 ;
		unlink(base, next) ; }
	if (*prev & 1) {
			deb = **(prev-1) ;
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

// Insère un bloc dans le système de ségrégation
void link(uint8** base, uint8* bloc, uint32 n) ;

// Retire un bloc du système de ségrégation
void unlink(uint8** base, uint8* bloc) ;

// Renvoie un bloc libre de taille >=n, NULL si inexistant
uint8* find_bloc(uint8** base, uint32 n) ;

