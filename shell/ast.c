
#include "ast.h"

cmd_t init_cmd(cmd_case c, cmd_t* l, cmd_t* r, char* a)
{
	cmd_t cmd ;
	cmd.cs = c ;
	cmd.left = l ;
	cmd.right = r ;
	cmd.arg = a ;
	return cmd ;
}

