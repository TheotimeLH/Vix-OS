#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include "../stdlib/stdlib.h"
#include "../common/malloc.h"

// On doit définir une structure de trie
// On doit vérifier qu'aucun des pattern n'est sous-mot d'un autre (si c'est le cas, on garde que le plus petit )
// Ou alors on peut rajouter un espace à la fin de chaque pattern

typedef struct trie_node
{
	int root; // 1 si c'est la racine du trie
	struct trie_node* prev;
	struct trie_node* next[256];
	struct trie_node* fail_link;
	int leaf; // 1 si c'est le bout 
} trie_node_t;

typedef struct trie_iterator
{
	trie_node_t* root;
	trie_node_t* it;
} trie_iterator_t;
trie_node_t* build_trie(int n, char *patterns[]);
int add_char(trie_iterator_t* iterator, char c);
void test_aho_corasick();

#endif
