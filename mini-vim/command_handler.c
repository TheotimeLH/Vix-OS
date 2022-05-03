#include "command_handler.h"


static pair_t split_on(char* buff, char c)
{
	int i = 0;
	char* r = buff;
	while(buff[i])
	{
		r++;
		if(buff[i] == c)
		{
			buff[i] = 0;
			break;
		}
		i++;
	}
	return (pair_t) {.fst = (void*) buff, .snd = (void*) r}; // On retourne la paire de pointeurs qui pointent vers le premier et le deuxieme
}

static int strcmp(const char* a,const char* b)
{
    for(int i=0;;i++)
    {
        if(a[i]!=b[i])
            return 0;
        if(a[i]==0)
            break;
    }
    return 1;
}
uint32 interpret_command(char* com_buff, line_t* curr_buff, line_t* scr_start)
{
	// Déja on doit déterminer les différents éléments
	pair_t res = split_on(com_buff, ' ');
	char* comm = (char*) res.fst;
	char* parm = (char*) res.snd;
	if(strcmp(comm, "e"))
	{
		// On doit ouvrir un fichier
		*curr_buff = buffer_from_file(parm, 100); // On dit de taille 1000, à changer
		*scr_start = *curr_buff;
		return 1; // On va faire un bitmask pour dire ce qui a été modifié
	}
	if(strcmp(comm, "q"))
	{
		// Il faudrait flush aussi
		return 2;
	}
	

}
