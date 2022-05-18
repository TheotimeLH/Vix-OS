#ifndef SYNTAX_H
#define SYNTAX_H
#include "../stdlib/stdlib.h"
#include "../common/malloc.h"
#include "structures.h"
#include "aho_corasick.h"



typedef struct syntax_param
{
	trie_iterator_t trie;
	vga_color fg;
	vga_color bg;
} syntax_param_t;

typedef struct syntax_config
{
	int n;
	syntax_param_t* conf;
} syntax_config_t;

void syntax_from_start(file_t* file, syntax_config_t syntax);

#endif
