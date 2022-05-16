#ifndef AUTOMATA_H
#define AUTOMATA_H

#include "../stdlib/stdlib.h"
#include "../common/malloc.h"
#include "structures.h"

typedef enum direction
{
	STILL,
	UP,
	DOWN,
	RIGHT,
	LEFT
} direction_t;

typedef struct command
{
	mode_t new_mode;
	int del : 3; // Est ce que ça sert à supprimer qqch 
							// le deuxieme bit sert à savoir si on supprime un caractere ou toute une ligne
	int mov : 2; // Est ce que c'est associé à un mouvement
	direction_t dir;
} command_t;


typedef struct node
{
	int id;
	int final_state : 1; // Est ce que c'est un état final
	struct node* transition[256];
	int command_number;  // 0 si aucune commande sinon correspondance dans le tableau
} node_t;

typedef struct automata
{
	node_t* initial_state;
	node_t* current_state;
	
} automata_t;

automata_t new_automata();
void add_command(automata_t* automaton ,command_t command, char* lex);
command_t* enter_char(automata_t* automaton, char c);

#endif
