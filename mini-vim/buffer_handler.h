#ifndef BUFFER_HANDLER_H
#define BUFFER_HANDLER_H

#include "structures.h"
#include "../stdlib/stdlib.h"

line_t* buffer_from_file(char* filename, uint32 size);
void save_to_file(char* filename, line_t* file);


#endif
