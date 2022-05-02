#include "command_handler.h"


static char* split_on(char* buff, char c)
{
	int i = 0;
	char* r = buff;
	while(buffer[i])
	{
		r++;
		if(buffer[i] == c)
		{
			r++;
			buffer[i] = 0;
			break;
		}
		i++;
	}
	return (pair_t) {(void*) buff, (void*) r}; // On retourne la paire de pointeurs qui pointent vers le premier et le deuxieme
}

void interpret_command(char* com_buff, line_t* curr_buff, line_t* scr_start)
{
	// Déja on doit déterminer les différents éléments
	pair_t res = split_on(com_buff, ' ');
	char* comm = (char*) res.fst;
	char* parm = (char*) res.snd;
	

}
