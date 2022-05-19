#ifndef MALLOC_H
#define MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

void init_tas();
void *malloc(uint32 n);
void free(void* bloc);

#ifdef __cplusplus
}
#endif
#endif
