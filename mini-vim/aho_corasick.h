#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include "../common/malloc.h"

// On doit définir une structure de trie
// On doit vérifier qu'aucun des pattern n'est sous-mot d'un autre (si c'est le cas, on garde que le plus petit )
// Ou alors on peut rajouter un espace à la fin de chaque pattern

typedef struct trie_node
{
	int root; // 1 si c'est la racine du trie
	struct trie_node* prec;
	struct trie_node* next[256];
	struct trie_node* fail_link;
	int leaf; // 1 si c'est le bout 
} trie_node_t;

#endif
