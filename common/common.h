#ifndef COMMON_H
#define COMMON_H


typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned long long uint64;

typedef uint32 uint32_t;
typedef uint16 uint16_t;
typedef uint8 uint8_t;

#include <stddef.h>
//#include <stdbool.h> probleme de compatibilité car inclus string.h
#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0

#endif

#ifdef __cplusplus
extern "C" {
#endif

void *memset(void *pointer, int value, size_t count); //__THROW __nonnull((1));
void outb(uint8 value, uint16 port);
void outw(uint16 value, uint16 port);
uint8 inb(uint16 port);
uint16 inw(uint16 port);
void itoa(int num, char *number);
uint32 strlen(const char* str);
uint32 digit_count(int num);
void print_hexa(uint32 x);
int strcmp(const char* a,const char* b);

void cli();
void sti();
uint32 get_esp();

#ifdef __cplusplus
}
#endif


#endif
