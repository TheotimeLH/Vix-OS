%{
	#include "ast.h"

	cmd ret ;
	cmd_t init_cmd(cmd_case c, void* f, void* s)
	{
		ret.cs = c ;
		ret.fst = f ;
		ret.snd = s ;
		return ret ;
	}
%}

%token <char*> ID
%token SEQ
%token OR
%token AND
%token PIPE
%token EOL

%type <cmd_t> commande

%%

commande: %empty					{ $$ = & init_cmd(SKIP, NULL, NULL) }
	| ID ID			 						{ $$ = & init_cmd(EXEC, $1, $3) }
	| commande SEQ commande	{ $$ = & init_cmd(SEQ, $1, $3) }
	| commande OR commande 	{ $$ = & init_cmd(OR, $1, $3) }
	| commande AND commande	{ $$ = & init_cmd(AND, $1, $3) }
	| ID PIPE commande	 		{ $$ = & init_cmd(PIPE, $1, $3) }				
	| LPAR commande RPAR		{ $$ = $2 }

%%

cmd_t parse()
{
	if (yyparse() == 0) return ret ;
	// TODO : error handling
}
