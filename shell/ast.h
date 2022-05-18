
#ifndef SHELL_H
#define SHELL_H

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

#endif 

