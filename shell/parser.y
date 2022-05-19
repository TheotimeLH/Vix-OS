%{
	#include "shell.h"

	int yyerror() { return 0 ; }

	char next_char()
	{
		char str[2] = "\0" ;
		keyboard_t kb = get_keyboard() ;
		if (kb.type == 0) str[0] = kb.k.ch ;
		else if (kb.k.sp == SPACE) str[0] = ' ' ;
		else if (kb.k.sp == TAB) str[0] = '\t' ;
		else if (kb.k.sp == ENTER) str[0] = '\n' ;
		else return next_char() ;
		write(0, str) ;
		return str[0] ;
	}

	char left = '\0' ;		
	char buf[100] ;
	int k=0 ;

	cmd_t ret ;
	cmd_t init_cmd(cmd_case c, void* f, void* s)
	{
		ret.cs = c ;
		ret.fst = f ;
		ret.snd = s ;
		return ret ;
	}
%}

%token <str> ID
%token PVIRG
%token BAR
%token ESP
%token EOL
%token LPAR
%token RPAR

%left PVIRG
%left BAR
%left ESP

%type <cmd_t> commande
%start commande

%%

commande:%empty								{ $$ = init_cmd(SKIP, NULL, NULL) ; }
	|ID 												{ $$ = init_cmd(EXEC, $1, NULL) ; }
	|ID ID			 								{ $$ = init_cmd(EXEC, $1, $2) ; }
	|commande PVIRG commande		{ $$ = init_cmd(SEQ, &$1, &$3) ; }
	|commande BAR BAR commande 	{ $$ = init_cmd(OR, &$1, &$4) ; }
	|commande ESP ESP commande	{ $$ = init_cmd(AND, &$1, &$4) ; }
	|LPAR commande RPAR					{ $$ = $2 ; }
;

%%

int yylex() 
{
	char c ;
	char str[k+1] ;
	if (left != '\0')	c = left ;
	else c = next_char() ;
	left = '\0' ;

	if (k>0)
		switch(c) {
			case '\n':
			case '(':
			case ')':
			case ';':
			case '|':
			case '&':
				left=c ;
			case '\t':
			case ' ': 
				buf[k++] = '\0' ;
				for (int i=0 ; i<k ; i++) str[k] = buf[k] ;
				yylval.str = str ;
				return ID ;
			default:
				buf[k++]=c ;
				return yylex() ;
		}
	else
		switch (c) {
			case '\n': return NULL ;
			case '(': return LPAR ;
			case ')': return RPAR ;
			case ';': return PVIRG ;
			case '|': return BAR ;
			case '&': return ESP ;
			case '\t':
			case ' ':
				return yylex() ;
			default:
				buf[k++]=c ;
				return yylex() ;
		}		
}
	
cmd_t parse_cmd()
{
	ret = init_cmd(SKIP, NULL, NULL) ;
	if (yyparse() == 0) return ret ;
	write(0, "ECHEC\n") ;
	return parse_cmd() ;
}

