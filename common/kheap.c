#include "common.h"

uint32 kmalloc_int(uint32 sz, int align, uint32 *phys){
	if(align == 1 && (placement_adress & 0xFFFFF000)){ // Si l'adresse est pas alignée
		placement_adress &= 0xFFFFF000;
		placement_adress += 0x1000;
	}
	if(phys){
		*phys = placement_adress;
	}
	uint32 tmp = placement_adress;
	placement_adress += sz;
	return tmp;
}


uint32 kmalloc(uint32 sz){ // De base
	return kmalloc_int(sz, 0, 0);
}

uint32 kmalloc_a(uint32 sz){ // Mode aligné
	return kmalloc_int(sz, 1, 0);
}
uint32 kmalloc_p(uint32 sz, uint32 *phys){ // adresse physique
	return kmalloc_int(sz, 0, phys);
}
uint32 kmalloc_ap(uint32 sz, uint32 *phys){
	return kmalloc_int(sz, 1, phys);
}
