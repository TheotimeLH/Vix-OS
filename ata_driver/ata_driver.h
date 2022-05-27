//#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>
#include "../common/common.h"
#include "../video/vga_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Drive_id
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
bool ata_write(Drive d,uint8_t count,uint32_t lba,uint16_t *buffer);//pareil

#ifdef __cplusplus
}
#endif
