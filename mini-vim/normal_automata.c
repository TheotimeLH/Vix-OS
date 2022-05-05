#include "normal_automata.h"

command_t commands[256];
int command_index = 1; // le 0 sert à mettre du vide
int possible_id = 0;
static node_t* new_node(int final)
{
	node_t* state = (node_t*) malloc(sizeof(node_t));
	state->id = possible_id ++;
	state->final_state = final;
	memset(state->transition, 0, sizeof(node_t*) * 256); // on met que des 0
	return state;
}

automata_t new_automata()
{
	node_t* initial_state = new_node(0);
	return (automata_t) {initial_state, initial_state};
}

void add_command(automata_t* automaton, command_t command, char* lex)
{
	node_t* current = automaton->initial_state;
	int i = 0;
	while(lex[i] && current->transition[lex[i]])
	{
		current = current->transition[lex[i]];
		i++;
	}
	if(lex[i] == 0)
	{ // On est à la fin du lexeme
		current->final_state = 1;
		current->command_number = command_index;
		commands[command_index++] = command; 
		return;
	}
	while(lex[i])
	{
		node_t* prochain = new_node(0);
		current->transition[lex[i++]] = prochain;
		current = prochain;
	}
	current->final_state = 1;
	current->command_number = command_index;
	commands[command_index++] = command;
	
	// on est arrivés à la fin de l'arbre
}
