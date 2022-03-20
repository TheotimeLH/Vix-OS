#ifndef COMMON_H
#define COMMON_H

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;


void outb(uint8 value, uint16 port);
uint8 inb(uint16 port);
uint16 inw(uint16 port);
void itoa(int num, char *number);
uint32 strlen(const char* str);
uint32 digit_count(int num);


#endif
