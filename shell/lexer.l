%{
	#include "ast.c"
%}

char [a-zA-Z0-9]|_|.|-|/
ident char*

%%

{ident} { return ID ; }
; 			{ return SEQ ; }
"||" 		{ return OR ; }
"&&"		{ return AND ; }
| 			{ return PIPE ; }
(				{ return LPAR ; }
)				{ return RPAR ; }
\n 			{ return EOL ; }

%%
