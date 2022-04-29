#ifndef PAGING_H
#define PAGING_H
#include "common.h"
#include "isr.h"

typedef struct page
{
	uint32 present : 1; // La page est presente en mémoire
	uint32 rw      : 1; // Lecture seule si 0 sinon lecture et écriture
	uint32 user    : 1; // 
	uint32 accessed: 1; // Est ce qu'on a accédé à la page depuis le dernier rafraichissement
	uint32 dirty   : 1; // Est ce qu'on a écrit sur la page depuis le dernier rafraichissement
	uint32 unused  : 7;
	uint32 frame   : 20; // Adresse de la frame (shifté de 12 bits sur la droite )
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t *tables[1024];
	uint32 tablesPhysical[1024];
	uint32 physicalAddr;
} page_directory_t;

void initialise_paging();
void switch_page_directory(page_directory_t *new);

page_t get_page(uint32 address, int make, page_directory_t *dir);


/* 
 * Ce qui gere les page faults
 */
void page_fault(registers_t regs);

#endif
