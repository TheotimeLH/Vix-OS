#include "syntax.h"

syntax_config_t create_config(int n, syntax_param_t confs[])
{
	return (syntax_config_t) {n, confs};
}


void syntax_from_start(file_t* file, syntax_config_t syntax)
{
	line_t* file_buff = file->file_buffer;
	for(int i = 0; i < syntax.n; i ++)
	{
		syntax_param_t par = syntax.conf[i];
		while(file_buff != 0)
		{
			text_list_t* line_buffer = file_buff->line_buffer;
			while(line_buffer != 0)
			{
				if(add_char(&par.trie, line_buffer->e.c)) // Il faut recuperer ou est le début
				{
					line_buffer->e.bg = par.bg;
					line_buffer->e.fg = par.fg;
				}
				line_buffer = line_buffer->next;
			}
			add_char(&par.trie, '\n');
			file_buff = file_buff->next;
		}
	}
}
