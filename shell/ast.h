
#ifndef SHELL_H
#define SHELL_H

typedef enum {
	SKIP,
	SYST,
	SEQ,
	AND,
	OR,
	PIPE
} cmd_case ;

typedef struct {
	cmd_case cs ;
	void* left ;
	void* right ;
} cmd_t ;

#endif 

