#include "common.h"

// Gestion des ports
void outb(uint8 value, uint16 port){
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void outw(uint16 value, uint16 port){
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

uint8 inb(uint16 port){
	uint8 ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

uint16 inw(uint16 port){
	uint16 ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

// Misc 
uint32 digit_count(int num){
	uint32 count = 0;
	if(num == 0)
		return 1;
	else{
		while(num){
			count ++;
			num /= 10;
		}
		return count;
	}
}

uint32 strlen(const char* str){
	uint32 length = 0;
	while(str[length++]);
	return length;
}

void itoa(int num, char* number){
	int dg = digit_count(num);
	int i = dg -1;
	if(num == 0 && dg == 1){
		number[0] = '0';
		number[1] = '\0';
	}
	else{
		while(num){
			char x = num%10;
			number[i--] = x + '0';
			num /= 10;
		}
		number[dg] = '\0';
	}
}
