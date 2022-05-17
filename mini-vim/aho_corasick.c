#include "aho_corasick.h"

trie_node_t* build_trie(int nb, char *patterns[])
{
	trie_node_t* ans = (trie_node_t*) malloc(sizeof(trie_node_t));
	ans->root = 1;
	ans->prec = 0;
	for(int i = 0; i < nb ; i++) // On construit d'abord l'arbre
	{
		int j = 0; 
		char* pat = patterns[i];
		trie_node_t* curr = ans;
		while(pat[j] != '\0')
		{
			char c = pat[j];
			if(curr->next[c])
			{
				curr = curr->next[c];
			}
			else
			{
				trie_node_t* nouveau = (trie_node_t*) malloc(sizeof(trie_node_t));
				curr->next[c] = nouveau;
				nouveau->prec = curr;
				nouveau->root = 0;
				curr = nouveau;
				if(pat[j+1] == '\0')
					curr->leaf = 1;
			}
			j++;
		}
	}
	// puis on ajoute les failure links
	


	return ans;
}


int add_char(trie_node_t* curr_node, char c)
{
	if(curr_node->next[c])
	{
		curr_node = curr_node->next[c];
		if(curr_node->leaf)
		{
			return 1;
		}
		return 0;
	}
	else
	{
		if(!curr_node->root)
		{
			curr_node = curr_node->fail_link;
		}
		return 0;
	}
}



