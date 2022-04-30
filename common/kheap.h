#ifndef KHEAP_H
#define KHEAP_H
#include "common.h"

uint32 kmalloc(uint32 sz); // De base
uint32 kmalloc_a(uint32 sz); // Mode aligné
uint32 kmalloc_p(uint32 sz, uint32 *phys); // adresse physique
uint32 kmalloc_ap(uint32 sz, uint32 *phys);
#endif
