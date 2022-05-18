
#ifndef SHELL_H
#define SHELL_H

#include "../stdlib/stdlib.h"

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

cmd_t parse_cmd() ;

int eval_cmd(cmd_t c) ;

#endif 

