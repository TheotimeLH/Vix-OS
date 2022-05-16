%{
	#include "ast.c"
%}

%token <char*> ID
%token SEQ
%token OR
%token AND
%token PIPE
%token EOL

%%

ligne: commande EOL { $$ = $1 }

commande: %empty				{ $$ = init_cmd(SKIP, NULL, NULL) }
	| ID ID			 					{ $$ = init_cmd(SYST, &$1, &$2) }
	| ID SEQ ID 		 			{ $$ = init_cmd(SEQ, &$1, &$2) }
	| ID OR ID 		 				{ $$ = init_cmd(OR, &$1, &$2) }
	| ID AND ID 			 		{ $$ = init_cmd(AND, &$1, &$2) }
	| ID PIPE ID	 		 		{ $$ = init_cmd(PIPE, &$1, &$2) }				
	| LPAR commande RPAR	{ $$ = $2 }

%%
