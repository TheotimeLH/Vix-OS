#ifndef _FAT_DRIVER_H_
#define _FAT_DRIVER_H_

#include <stddef.h>
#include <stdint.h>
#include "../ata_driver/ata_driver.h"
#include "../video/vga_driver.h"

typedef struct Fat_infos Fat_infos;

class Fat_system
{
public:
    virtual bool read(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512)=0;
    virtual bool write(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512)=0;
    virtual void err(char* msg)=0;
    virtual uint32_t get_sectorsize(){return 512;};
};

class Ata_fat_system:public Fat_system
{
public:
    Ata_fat_system(Drive d);
    virtual bool read(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512);
    virtual bool write(uint8_t count,uint32_t addr,uint8_t* buffer,uint16_t sector_size=512);
    virtual void err(char* msg);
    bool is_ready(){return m_id.exists&&!m_id.erreur;};
    virtual uint32_t get_sectorsize(){return m_id.taille_secteur;};
private:
    Drive_id m_id;
    Drive m_drive;
};

class Fat_entry
{
public:
    Fat_entry(){};
    Fat_entry(uint32_t cluster,bool is_directory,char* name,uint32_t size)
    :m_is_directory(is_directory),m_first_cluster(cluster),m_current_cluster(cluster),
    m_current_entry_offset(0),m_last_entry(0),m_size(size)
    {
        m_name[8]='\0';
        for(int i=0;i<8;i++)
        {
            m_name[i]=name[i];
            if(m_name[i]==0||m_name[i]==' ')
            {
                m_name[i]=0;
                break;
            }
        }
    };
    const char* get_name(){return m_name;};
    const bool is_directory(){return m_is_directory;};
    void init_offset(){m_current_cluster=m_first_cluster;m_current_entry_offset=0;m_last_entry=false;};
    const uint32_t get_size(){return m_size;};

    //uniquement pour les fichiers
    //renvoit la taille lue en octet
    //si elle est inférieure à cluster_count*infos.byte_per_cluster (la taille du buffer),
    //c'est que la fin du fichier a été atteinte
    //utiliser init_offset pour repartir du début
    //attention, le buffer doit être de taille cluster_count*infos.byte_per_cluster
    uint32_t read_data(uint8_t *buffer,uint32_t cluster_count,Fat_infos* infos,Fat_system* intf);

    //uniquement pour les répertoires
    //renvoit le nombre d'entrées lues, s'il est inférieur à size,
    //c'est que toutes les entrées du répertoire ont été lues
    //(size est la taille du buffer)
    //utiliser init_offset pour repartir du début
    //taille de buffer conseillée :
    //  - pour la racine en fat12/16 : infos.sector_size/32
    //  - sinon, infos.byte_per_cluster/32
    uint32_t read_entries(Fat_entry* buffer,uint32_t size,Fat_infos* infos,Fat_system* intf);

private:
    uint32_t m_first_cluster;//0 for root (FAT12/16)
    char m_name[9];
    bool m_is_directory;
    uint32_t m_current_cluster;//sector offset for fat12/16 root
    uint32_t m_size;//en octets
    uint32_t m_current_entry_offset;//for directory
    bool m_last_entry;//..
};

Fat_infos fat_init(Fat_system* intf);

struct Fat_infos
{
    uint8_t fat_type;//12,16,32
    uint16_t sector_size;//en octet
    uint8_t cluster_size;//en secteurs
    uint32_t byte_per_cluster;//cluster size en octet
    uint32_t fat;//en secteurs
    uint32_t fat_size;//...
    uint32_t root;//en secteur pour 12/16 et en cluster pour 32
    uint32_t root_size;//en secteur
    uint32_t data;//...
    uint32_t data_size;//...
    Fat_entry root_fat_entry;
};

#endif
