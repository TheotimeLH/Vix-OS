#include "aho_corasick.h"

trie_node_t* build_trie(int nb, char *patterns[])
{
	trie_node_t* ans = (trie_node_t*) malloc(sizeof(trie_node_t));
	ans->root = 1;
	ans->prev = 0;
	for(int i = 0; i < 256; i++)
		ans->next[i] = (void*) 0;
	//write(0, "ROOT EN ");
	//print_hexa(ans);
	//putchar('\n');
	for(int i = 0; i < nb ; i++) // On construit d'abord l'arbre
	{
		int j = 0; 
		char* pat = patterns[i];
		trie_node_t* curr = ans;
		//write(0, "Nouveau pattern\n");
		while(pat[j] != '\0')
		{
			char c = pat[j];
			//write(0, "Caractere traite :");
			//putchar(c);
			//write(0, "\n");
			if(curr->next[c])
			{
				curr = curr->next[c];
			}
			else
			{
				trie_node_t* nouveau = (trie_node_t*) malloc(sizeof(trie_node_t));
				for(int i = 0; i < 256; i++)
					nouveau->next[i] = (void*) 0;
				curr->next[c] = nouveau;
				//write(0, "Lien de : ");
				//print_hexa(curr);
				//write(0, " vers : ");
				//print_hexa(nouveau);
				//putchar('\n');
				nouveau->prev = curr;
				nouveau->root = 0;
				nouveau->fail_link = (void*) 0;
				curr = nouveau;
				if(pat[j+1] == '\0')
				{
					curr->leaf = 1;
					//write(0, "On cree une leaf :");
					//print_hexa((uint32) curr);
					//write(0, "\n");
				}
			}
			j++;
		}
	}
	// puis on ajoute les failure links
	ans->fail_link = ans; // pratique 
	int continuer = 1;
	trie_node_t* tab[256*256]; // bon faudra peut etre augmenter à un moment
	int m = 1;
	tab[0] = ans;
	while(m)
	{
		trie_node_t* new_tab[256*256]; // peut faire crasher le code ...
		char text_corr[256*256];
		int k = 0;
		for(int i = 0; i < m; i++)
		{
			for(int j = 0; j < 256; j++)
			{
				if(tab[i]->next[j]){
					new_tab[k] = tab[i]->next[j];
					text_corr[k++] = j;
				}
			}
		}
		// On va travailler sur les trucs qu'on a 
		for(int i = 0; i < k ; i++)
		{
			trie_node_t* actuel = new_tab[i];
			trie_node_t* chemin = actuel->prev->fail_link; // normalement le link est bon
			while(!chemin->next[text_corr[i]] && !chemin->root)
			{
				chemin = chemin->fail_link;
			}
			actuel->fail_link = chemin->next[text_corr[i]];
		}
		for(int i = 0; i < 256*256; i ++)
		{
			tab[i] = new_tab[i];
		}
		m = k;
	}
	return ans;
}


int add_char(trie_iterator_t* iterator, char c)
{
	//write(0, "CARAC : ");
	//putchar(c);
	//putchar('\n');
	//write(0, "FORCING");
	//print_hexa(iterator->it->next['b']);
	//write(0, " FINI \n");
	if(iterator->it->next[c])
	{
		//write(0, "On avance\n");
		iterator->it = iterator->it->next[c];
		//print_hexa(iterator->it);
		//write(0, " et ");
		//print_hexa(iterator->it->root);
		//write(0, "\n");
		if(iterator->it->leaf)
		{
			iterator->it = iterator->root;
			return 1;
		}
		return 0;
	}
	else
	{
		if(!iterator->it->root)
		{
			//write(0, "On recule");
			iterator->it = iterator->it->fail_link;
		}
		else
		{
			//write(0, "On est à la root");
		}
		return 0;
	}
}


void test_aho_corasick() // une petite fonction pour tester aho_corasick
{
	char* pat[2];
	pat[0] = "abcd";
	pat[1] = "cdef";
	trie_iterator_t truc;
	//trie_node_t* trie = build_trie(1, pat);
	//int a = add_char(trie, 'a');
	//int b = add_char(trie, 'b');
	//int c = add_char(trie, 'c');
	//int d = add_char(trie, 'd');
	//print_hexa(a);
	//print_hexa(b);
	//print_hexa(c);
	//print_hexa(d);
	write(0, "TEST AHO :");
	pat[0] = "aa";
	pat[1] = "cad";
	//write(0, "\nc\n");
	int a;
	//int c = add_char(trie, 'd');
	//print_hexa(a);
	//print_hexa(b);
	//print_hexa(c);
	
	pat[0] = "camions";
	pat[1] = "les";
	truc.it = build_trie(2, pat); 
	truc.root = truc.it;
	char texte[30] = "Je collecte le camions";
	int i = 0;
	a = 0;
	while(texte[i])
	{
		int c = add_char(&truc, texte[i++]);
		if(c)
		{
			write(0, "fin de mot a ");
			putchar(texte[i-1]);
			putchar('\n');
		}
		a = a + c;
	}
	if(a)
	{
		write(0, "TEST VALIDE\n");
		print_hexa(a);
	}

	write(0, "\nFIN TEST\n");
}


