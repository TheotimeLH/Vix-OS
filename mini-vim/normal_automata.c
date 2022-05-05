include "normal_automata.h"

int commands[256] = {};
int command_index = 1; // le 0 sert à mettre du vide
int possible_id = 0;
static node_t* new_node(int final)
{
	node_t* state = (node_t*) malloc(sizeof(node_t));
	state->id = possible_id ++;
	state->final_state = final;
	state->transition = (node_t**) malloc(sizeof(node_t*)*256);
	return state;
}

automata_t new_automata()
{
	node_t* initial_state = new_node(0);
	return (automata_t) {initial_state, initial_state};
}

void add_command(automata_t automaton, command_t command, char* lex)
{

}
