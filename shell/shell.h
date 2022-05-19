
#ifndef SHELL_H
#define SHELL_H

#include "../stdlib/stdlib.h"
#include "../common/malloc.h"

typedef char* str ;

typedef enum {
	SKIP,
	EXEC,
	SEQ,
	AND,
	OR,
	PIPE
} cmd_case ;

typedef struct {
	cmd_case cs ;
	void* fst ;
	void* snd ;
} cmd_t ;

union yystype{
	str str ;
	cmd_t cmd_t ;
} ;

#define NULL 0
#define YYSTYPE union yystype
#define YYSIZE_T uint32

#endif 

