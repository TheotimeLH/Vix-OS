
#include "ast.h"

int exec_cmd(char** fun, char** arg)
{
	return 1 ; // TODO
}

int eval_cmd(cmd_t c)
{
	if (c.cs == SKIP) return 0 ;
	if (c.cs == SYST) return exec_cmd(cs.fst, cs.snd) ;
	if (c.cs == PIPE) return 1 ; // TODO
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

