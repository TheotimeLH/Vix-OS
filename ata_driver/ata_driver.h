#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern void outb(uint16_t port,uint8_t data);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

typedef struct
{
	bool exists;
	bool erreur;
	uint32_t nb_secteur;
	uint32_t taille_secteur;
} Drive_id;

typedef enum
{
    hda=0b00,
    hdb=0b01,
    hdc=0b10,
    hdd=0b11
} Drive;

Drive_id ata_identify(Drive d);
bool ata_read(Drive d,uint8_t count,uint32_t lba,uint16_t* buffer);//true si la lecture s'est bien passée