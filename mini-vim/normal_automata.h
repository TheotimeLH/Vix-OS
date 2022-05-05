#ifndef AUTOMATA_H
#define AUTOMATA_H

typedef enum direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
} direction_t;

typedef struct command
{
	int del : 1; // Est ce que ça sert à supprimer qqch
	int mov : 1; // Est ce que c'est associé à un mouvement
	direction movement;
} command_t;

typedef struct transition_function
{
	
}

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

#endif
