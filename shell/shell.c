
#include "shell.h"
#include "parser.h"

cmd_t parse_cmd() ;

int eval_cmd(cmd_t c)
{
	if (c.cs == SKIP) return 0 ;
	if (c.cs == PIPE) return 1 ; // TODO

	if (c.cs == EXEC) {
		if (c.snd == NULL) exec(c.fst) ;
		else execa(c.fst, c.snd) ;
		return 0 ;
	}
	cmd_t left = *(cmd_t*) c.fst ;
	cmd_t right = *(cmd_t*) c.snd ;

	if (c.cs == SEQ) {
		eval_cmd(left) ;
		return eval_cmd(right) ;
	}
	if (c.cs == AND) {
		int a = eval_cmd(left) ;
		if (a!=0) return a ;
		else return eval_cmd(right) ;
	}
	if (c.cs == OR)	{
		int a = eval_cmd(left) ;
		if (a==0) return a ;
		else return eval_cmd(right) ;
	}
}

int main()
{
	while (1)
	{
		cmd_t cmd ;
		cmd = parse_cmd() ;
		if (eval_cmd(cmd) != 0)
			write(0, "ERREUR\n") ;
	}	
}

