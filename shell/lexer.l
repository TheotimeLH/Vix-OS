
%option noyywrap

%{
	#include "shell.h"

	char next_char()
	{
		keyboard_t kb = get_keyboard() ;
		if (kb.type == 0) return kb.k.ch ;
		if (kb.k.sp == SPACE) return ' ' ;
		if (kb.k.sp == TAB) return '\t' ;
		if (kb.k.sp == ENTER) return '\n' ;
		else return next_char() ;
	}
	
	#define YY_INPUT(buf, result, max_size) 								\
		{ 																										\
   		char c = next_char() ; 															\
			write(0, "c") ; 																		\
    	result = (c == '\n') ? YY_NULL : (buf[0] = c, 1) ; 	\
    }
%}

char [a-zA-Z0-9]|_|.|-|\/
ident char*
space [ \t]

%%

{ident} { return ID ; }
; 			{ return SEQ ; }
\|\|		{ return OR ; }
&&			{ return AND ; }
\|			{ return PIPE ; }
\(			{ return LPAR ; }
\)			{ return RPAR ; }
{space} 

%%
